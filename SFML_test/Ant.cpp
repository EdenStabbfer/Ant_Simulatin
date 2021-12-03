#include <vector>

#include "Ant.h"

// Initialization
Ant::Ant(float maxSpeed) : speed(maxSpeed)
{
	this->position = Vector2d(Config::width / 2, Config::height / 2);
	this->desiredDir = randomVectorInCircle();
	this->velocity = desiredDir * maxSpeed;
	this->borders = Vector2d(Config::width, Config::height);

	this->speed = maxSpeed;
	this->target = TO_FOOD;
}

// Methods
void Ant::chooseDesiredDirection()
{
	desiredDir = (desiredDir + randomVectorInCircle() * Config::wonderStrength).normalize();
}

void Ant::update(std::vector<Pheromone>& pheromones, const float& dt, const float& pherDt, const Vector2d& foodPos, const Vector2d& homePos)
{
	std::vector<int> phAroundId;
	std::vector<float> product;

	// Проверяем достигли ли дома или еды. Если да, меняем направление и возвращаем true,  иначе - false
	bool homeFoodFound = findHomeFood(foodPos, homePos);

	// Считаем компоненты вероятности для феромонов и обновляем его, если в радиусе муравья
	float productSum(0);
	float oneProd(0);
	bool isAnyWithin(false);
	int cnt(0);
	float dist(0);
	for (auto& ph : pheromones)
	{
		dist = position.distanceTo(ph.x, ph.y);
		if (target != ph.type && dist < Config::antSize)
		{
			isAnyWithin = true;
			ph.value += Config::pheromoneUpdateRate / (speed * dt);
		}
		if (target == ph.type && dist < Config::radiusOfView && dist > Config::antSize)
		{
			if (this->velocity.angleTo(this->position.VectorTo(ph.x, ph.y)) < 1.f)
			{
				oneProd = pow(dist, Config::distanceStrength) * pow(ph.value, Config::pheromoneStrength);
				productSum += oneProd;
				product.push_back(oneProd);
				phAroundId.push_back(cnt);
			}
		}
		cnt++;
	}

	// Оставляем новую порцию феромона, если поблизости нет других и истёк таймер
	if (!isAnyWithin && pherDt > Config::phUpdateTime)
	{
		leavePheromone(pheromones);
	}
	
	// Выбираем направление
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> uid(0, 1);
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
					this->desiredDir = this->position.VectorTo(pheromones.at(phAroundId.at(i)).x, pheromones.at(phAroundId.at(i)).y).normalize();
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

	if (position.x > borders.x || position.x < 0)
	{
		desiredDir.x *= -1;
		velocity.x *= -1;
	}
	if (position.y > borders.y || position.y < 0)
	{
		desiredDir.y *= -1;
		velocity.y *= -1;
	}
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

bool Ant::findHomeFood(const Vector2d& foodPos, const Vector2d& homePos)
{
	float anyDist(0);
	if (this->target == TO_FOOD)
	{
		anyDist = this->position.distanceTo(foodPos);
		if (anyDist < Config::radiusOfView)
		{
			//std::cout << "food\n";
			if (anyDist < Config::antSize)
			{
				this->target = TO_HOME;
				this->velocity *= -1;
				this->desiredDir *= -1;
			}
			else
				this->desiredDir = this->position.VectorTo(foodPos).normalize();
			return true;
		}
	}
	else // this->target == TO_HOME
	{
		anyDist = this->position.distanceTo(homePos);
		if (anyDist < Config::radiusOfView)
		{
			//std::cout << "home\n";
			if (anyDist < Config::antSize)
			{
				this->target = TO_FOOD;
				this->velocity *= -1;
				this->desiredDir *= -1;
			}
			else
				this->desiredDir = this->position.VectorTo(homePos).normalize();
			return true;
		}
	}
	return false;
}

void Ant::leavePheromone(std::vector<Pheromone>& ph)
{
	if (this->target == TO_FOOD)
		ph.emplace_back(TO_HOME, 1.f, position.x, position.y);
	else
		ph.emplace_back(TO_FOOD, 1.f, position.x, position.y);
}