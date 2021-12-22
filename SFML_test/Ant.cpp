#include <vector>

#include "Ant.h"

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> uid(0, 1);

static std::vector<int> phAroundId;
static std::vector<float> product;

// Initialization
Ant::Ant(std::vector<Ant::Pheromone>* pheromones, std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* borders, float maxSpeed) : pheromones(pheromones), speed(maxSpeed)
{
	this->position = Vector2d(Config::width / 2, Config::height / 2);
	this->desiredDir = randomVectorInCircle();
	this->velocity = desiredDir * maxSpeed;
	this->borders = borders;

	this->coveredDistance = 0;

	this->target = TO_FOOD;
}

// Methods
void Ant::chooseDesiredDirection()
{
	desiredDir = (desiredDir + randomVectorInCircle() * Config::wonderStrength).normalize();
}

void Ant::update(const float& dt, const float& pherDt, const Vector2d& foodPos, const Vector2d& homePos)
{
	// Проверяем достигли ли дома или еды. Если да, меняем направление и возвращаем true,  иначе - false
	bool homeFoodFound = findHomeFood(foodPos, homePos);

	int cnt(0);
	float productSum(0);
	float oneProd(0);
	float sqDist(0);
	bool isPheromoneUnder(false);
	// Пробегаемся по всем феромонам
	for (auto& ph : *this->pheromones)
	{
		sqDist = position.sqDistanceTo(ph.position);
		// Если феромон под муравьём, обновляем его значение
		if (this->target != ph.type && sqDist < Config::sqAntSize)
		{
			isPheromoneUnder = true;
			ph.value = min(1.f, Config::pheromoneUpdateRate / this->coveredDistance + ph.value);
		}
		// Если расстояние до феромона меньше радиуса видимости, но больше размера муравья,
		if (!homeFoodFound && this->target == ph.type && sqDist < Config::sqRadiusOfView && sqDist > Config::sqAntSize)
		{
			// И если угол видимости меньше заданного, то выбираем эту точку 
			if (this->velocity.angleTo(this->position.VectorTo(ph.position)) < Config::visionAngle)
			{
				oneProd = pow(1/sqrt(sqDist), Config::distanceStrength) * pow(ph.value, Config::pheromoneStrength);
				productSum += oneProd;
				product.push_back(oneProd);
				phAroundId.push_back(cnt);
			}
		}
		cnt++;
	}

	// Оставляем новую порцию феромона, если поблизости нет других и истёк таймер
	if (!isPheromoneUnder && pherDt > Config::phUpdateTime)
		leavePheromone();
	
	// Выбираем направление
	if (!homeFoodFound)
	{
		if (!product.empty())
		{
			float chance = uid(gen);
			float prob(0);
			for (int i = 0; i < product.size(); i++)
			{
				prob += product.at(i) / productSum;
				if (chance <= prob)
				{
					this->desiredDir = this->position.VectorTo(this->pheromones->at(phAroundId.at(i)).position).normalize();
					break;
				}
			}
		}
		else
			chooseDesiredDirection();
	}

	// Движемся
	move(dt);

	product.clear();
	phAroundId.clear();
}

void Ant::move(const float& dt)
{
	Vector2d acceleration = (desiredDir * speed - velocity).normalize() * Config::streeringStrength;
	velocity = (velocity + acceleration * dt).normalize() * speed;
	position += this->velocity * dt;
	coveredDistance += speed * dt;

	if (position.x > Config::width || position.x < 0)
	{
		this->desiredDir.x *= -1;
		this->velocity.x *= -1;
	}
	if (position.y > Config::height || position.y < 0)
	{
		this->desiredDir.y *= -1;
		this->velocity.y *= -1;
	}
	// Коллизии
	Vector2d left, right;
	float A, B, dist;
	for (auto& pair : *this->borders)
	{
		left = this->position.VectorTo(pair.first);
		right = this->position.VectorTo(pair.second);
		if (sign(this->velocity.cross(left)) != sign(this->velocity.cross(right)))
		{
			left = this->position + left;
			right = this->position + right;
			A = right.y - left.y;
			B = left.x - right.x;
			dist = abs(this->position.x * A + this->position.y * B + left.y * right.x - right.y * left.x) / sqrt(A*A + B*B);
			if (dist < this->speed * dt)
			{	
				this->velocity *= -1;
				this->desiredDir *= -1;
			}
		}
	}
}

bool Ant::findHomeFood(const Vector2d& foodPos, const Vector2d& homePos)
{
	float sqDist(0);
	if (this->target == TO_FOOD)
	{
		sqDist = this->position.sqDistanceTo(foodPos);
		if (sqDist < Config::sqRadiusOfView)
		{
			if (sqDist < Config::sqAntSize)
			{
				this->target = TO_HOME;
				if (Config::turnOnTarget)
				{
					this->velocity *= -1;
					this->desiredDir *= -1;
				}
				this->coveredDistance = 0;
			}
			else
				this->desiredDir = this->position.VectorTo(foodPos).normalize();
			return true;
		}
	}
	else // this->target == TO_HOME
	{
		sqDist = this->position.sqDistanceTo(homePos);
		if (sqDist < Config::sqRadiusOfView)
		{
			if (sqDist < Config::sqAntSize)
			{
				this->target = TO_FOOD;
				if (Config::turnOnTarget)
				{
					this->velocity *= -1;
					this->desiredDir *= -1;
				}
				this->coveredDistance = 0;
			}
			else
				this->desiredDir = this->position.VectorTo(homePos).normalize();
			return true;
		}
	}
	return false;
}

void Ant::leavePheromone()
{
	if (this->target == TO_FOOD)
		this->pheromones->emplace_back(TO_HOME, Config::pheromoneLeaveRate / (Config::pheromoneLeaveRate + coveredDistance), this->position);
	else
		this->pheromones->emplace_back(TO_FOOD, Config::pheromoneLeaveRate / (Config::pheromoneLeaveRate + coveredDistance), this->position);
}

Ant::PheromoneType Ant::getTarget()
{
	return this->target;
}

Vector2d& Ant::getPosition()
{
	return this->position;
}

Vector2d& Ant::getVelocity()
{
	return this->velocity;
}

Vector2d& Ant::getDesiredDir()
{
	return this->desiredDir;
}
