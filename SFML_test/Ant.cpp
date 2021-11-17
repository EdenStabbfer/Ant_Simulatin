#include "Ant.h"


Ant::Ant(Vector2d startPos, float maxSpeed) : position(startPos), speed(maxSpeed) {}

void Ant::chooseDesiredDirection()
{
	this->dir = (this->dir + randomVectorInCircle() * wonderStrength).normalize();
}

void Ant::chooseSpecificDirection()
{

}

void Ant::move(const float dt)
{	
	this->chooseDesiredDirection();

	Vector2d streeringForce = (this->dir * speed - this->velocity).normalize() * streeringStrength;
	this->velocity = (velocity + streeringForce * dt).normalize() * speed;
	this->position += this->velocity;
}

Vector2d Ant::getPosition()
{
	return this->position;
}