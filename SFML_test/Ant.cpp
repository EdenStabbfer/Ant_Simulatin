#include <vector>

#include "Ant.h"

std::vector<Ant::Pheromone> pheromones;

// Initialization
Ant::Ant(const sf::Window& w, float maxSpeed)
{
	this->speed = maxSpeed;
	this->desiredDir = randomVectorInCircle();
	this->velocity = desiredDir * maxSpeed;
	this->borders = Vector2d(w.getSize().x, w.getSize().y);
	this->position = Vector2d(w.getSize().x / 2, w.getSize().y / 2);
	this->target = PheromoneType::TO_FOOD;
}

// Methods
void Ant::chooseDesiredDirection()
{
	desiredDir = (desiredDir + randomVectorInCircle() * wonderStrength).normalize();
}

void Ant::chooseSpecificDirection(const float& dt)
{
	static std::vector<std::reference_wrapper<Pheromone>> phAround;
	static std::vector<float> probability{};

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> uid(0, 1);

	float probabilitySum(0);
	float oneProb(0);

	for (auto& ph : pheromones)
	{
		if (ph.type == this->target)
		{
			float dist = this->position.distanceTo(ph.x, ph.y);
			if (dist < radiusOfView)
			{
				if (dist < antSize)
				{
					ph.value += pheromoneUpdateRatio / (speed * dt);
					continue;
				}
				oneProb = pow(1 / dist, distanceStrength) * pow(ph.value, pheromoneStrength);
				probabilitySum += oneProb;
				probability.push_back(oneProb);
				phAround.push_back(ph);
			}
		}
	}

	float chance = uid(gen);

	for (int i = 0; i < probability.size(); i++)
	{
		if (chance <= probability.at(i) / probabilitySum)
		{
			int x = std::remove_reference<Pheromone&>::type(phAround.at(i)).x;
			int y = std::remove_reference<Pheromone&>::type(phAround.at(i)).y;
			this->desiredDir = this->desiredDir.VectorTo(x, y);
			break;
		}
	}

	probability.clear();
	phAround.clear();
}

void Ant::move(const float dt)
{	
	chooseDesiredDirection();
	chooseSpecificDirection();
	//добавить испарение

	Vector2d streeringForce = (desiredDir * speed - velocity).normalize() * streeringStrength;
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

Vector2d Ant::getPosition()
{
	return this->position;
}

void Ant::leavePheromone()
{
	if (target == TO_FOOD)
		pheromones.emplace_back(TO_HOME, 1.f, position.x, position.y);
	else
		pheromones.emplace_back(TO_FOOD, 1.f, position.x, position.y);
}