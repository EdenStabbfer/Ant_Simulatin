#include <vector>

#include "Ant.h"

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

void Ant::chooseDirection(std::vector<Pheromone>& pheromones, const float& dt, sf::Clock& timer, const float& period)
{
	static std::vector<std::reference_wrapper<Pheromone>> phAround;
	static std::vector<float> probability{};

	float probabilitySum(0);
	float oneProb(0);
	bool isAnyWithin(false);
	for (auto& ph : pheromones)
	{
		if (ph.type == this->target)
		{
			float dist = this->position.distanceTo(ph.x, ph.y);
			if (dist < radiusOfView)
			{
				if (dist < antSize)
				{
					isAnyWithin = true;
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

	if (!isAnyWithin && timer.getElapsedTime().asSeconds() >= period)
	{
		leavePheromone(pheromones);
		timer.restart();
	}
	
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> uid(0, 1);
	int x, y;
	if (!probability.empty())
	{
		float chance = uid(gen);
		for (int i = 0; i < probability.size(); i++)
		{
			if (chance <= probability.at(i) / probabilitySum)
			{
				x = std::remove_reference<Pheromone&>::type(phAround.at(i)).x;
				y = std::remove_reference<Pheromone&>::type(phAround.at(i)).y;
				this->desiredDir = this->desiredDir.VectorTo(x, y).normalize();
				break;
			}
		}
	}
	else
		chooseDesiredDirection();
	probability.clear();
	phAround.clear();
}

void Ant::move(std::vector<Pheromone>& ph, const float& dt, sf::Clock& timer, const float& period)
{	
	chooseDirection(ph, dt, timer, period);
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

void Ant::leavePheromone(std::vector<Pheromone>& ph)
{
	if (target == TO_FOOD)
		ph.emplace_back(TO_HOME, 1.f, position.x, position.y);
	else
		ph.emplace_back(TO_FOOD, 1.f, position.x, position.y);
}