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
	this->target = PheromoneType::TO_FOOD;
}

// Methods
void Ant::chooseDesiredDirection()
{
	desiredDir = (desiredDir + randomVectorInCircle() * Config::wonderStrength).normalize();
}

void Ant::update(std::vector<Pheromone>& pheromones, const float& dt, const float& pherDt, const Vector2d& foodPos, const Vector2d& homePos)
{
	static std::vector<int> phAroundId{};
	static std::vector<float> probability{};

	// Проверяем достигли ли дома или еды. Если да, меняем направление и возвращаем true,  иначе - false
	bool homeFoodFound = findHomeFood(foodPos, homePos);

	// Считаем компоненты вероятности для феромонов и обновляем его, если в радиусе муравья
	float probabilitySum(0);
	float oneProb(0);
	bool isAnyWithin(false);
	int cnt(0);
	for (auto& ph : pheromones)
	{
		float dist = position.distanceTo(ph.x, ph.y);
		if (target != ph.type && dist < Config::antSize)
		{
			isAnyWithin = true;
			ph.value += Config::pheromoneUpdateRate / (speed * dt);
			continue;
		}
		if (target == ph.type && dist < Config::radiusOfView)
		{
			if (this->velocity.angleTo(this->position.VectorTo(ph.x, ph.y)) < 0.79f)
			{
				oneProb = pow(1 / dist, Config::distanceStrength) * pow(ph.value, Config::pheromoneStrength);
				probabilitySum += oneProb;
				probability.push_back(oneProb);
				phAroundId.push_back(cnt++);
			}
		}
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
		if (!probability.empty())
		{
			float chance = uid(gen);
			for (int i = 0; i < probability.size(); i++)
			{
				if (chance <= probability.at(i) / probabilitySum)
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

	probability.clear();
	phAroundId.clear();
}

void Ant::move(const float& dt)
{	
	//добавить испарение

	Vector2d streeringForce = (desiredDir * speed - velocity).normalize() * Config::streeringStrength;
	velocity = (velocity + streeringForce * dt).normalize() * speed;
	position += this->velocity;

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

Vector2d Ant::getPosition()
{
	return this->position;
}

bool Ant::findHomeFood(const Vector2d& foodPos, const Vector2d& homePos)
{
	float anyDist;
	if (this->target == TO_FOOD)
	{
		anyDist = this->position.distanceTo(foodPos);
		if (anyDist < Config::radiusOfView)
		{
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
			if (anyDist < Config::antSize)
			{
				this->target = TO_FOOD;
				this->velocity *= -1;
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