#include <vector>

#include "Ant.h"


static std::vector<Ant::Pheromone> pheromones;

// Initialization
Ant::Ant(const sf::Window& w, float maxSpeed, float radiusOfView)
{
	this->speed = maxSpeed;
	this->dir = randomVectorInCircle();
	this->velocity = dir * maxSpeed;
	this->borders = Vector2d(w.getSize().x, w.getSize().y);
	this->position = Vector2d(w.getSize().x / 2, w.getSize().y / 2);
	this->target = PheromoneType::TO_FOOD;
	this->radiusOfView = radiusOfView;
}

// Methods
void Ant::chooseDesiredDirection()
{
	dir = (dir + randomVectorInCircle() * wonderStrength).normalize();
}

void Ant::chooseSpecificDirection()
{

}

void Ant::move(const float dt)
{	
	this->chooseDesiredDirection();

	Vector2d streeringForce = (dir * speed - velocity).normalize() * streeringStrength;
	velocity = (velocity + streeringForce * dt).normalize() * speed;
	position += this->velocity;

	if (position.x > borders.x || position.x < 0)
	{
		dir.x *= -1;
		velocity.x *= -1;
	}
	if (position.y > borders.y || position.y < 0)
	{
		dir.y *= -1;
		velocity.y *= -1;
	}

}

Vector2d Ant::getPosition()
{
	return this->position;
}

void Ant::leavePheromone(float speed, float dt)
{
	if (target == TO_FOOD)
		pheromones.emplace_back(TO_FOOD, 1.f, position.copy());
	else
		pheromones.emplace_back(TO_HOME, 1.f, position.copy());
}