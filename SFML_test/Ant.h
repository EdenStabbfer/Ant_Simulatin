#pragma once

#include "Vector2d.h"


const float wonderStrength(12.f);
const float streeringStrength(8.f);

class Ant
{
private:
	Vector2d position;
	Vector2d dir;
	Vector2d velocity;
	float speed;

	void chooseDesiredDirection();

	void chooseSpecificDirection();


public:
	Ant(Vector2d startPos, float speed = 1.f);

	void move(const float dt);

	Vector2d getPosition();
};

