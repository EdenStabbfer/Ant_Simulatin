#pragma once

#include "Vector2d.h"

const float wonderStrength(12.f);
const float streeringStrength(8.f);
const float radiusOfView(12.f);
const float antSize(5.f);

const float pheromoneEvaporationRatio(0.1);
const float pheromoneUpdateRatio(0.01);
const int distanceStrength(5);
const int pheromoneStrength(5);


class Ant
{
public:
	Ant(const sf::Window& w, float maxSpeed = 1.f);

	enum PheromoneType
	{
		TO_HOME,
		TO_FOOD
	};

	struct Pheromone
	{
		PheromoneType type;
		float value;
		float x;
		float y;

		Pheromone(PheromoneType type, float value, float x, float y) :
			type(type), value(value), x(x), y(y){}
	};

private:
	Vector2d position;
	Vector2d desiredDir;
	Vector2d velocity;
	Vector2d borders;
	float speed;
	float radiusOfView;

	PheromoneType target;


	void chooseDesiredDirection();

	void chooseSpecificDirection(const float& dt);

public:
	void move(const float dt);

	void leavePheromone();

	Vector2d getPosition();
};

