#pragma once

#include "Vector2d.h"

const float wonderStrength(12.f);
const float streeringStrength(8.f);

class Ant
{
public:
	Ant(const sf::Window& w, float maxSpeed = 1.f, float radiusOfView = 10);

	enum PheromoneType
	{
		TO_HOME,
		TO_FOOD
	};

	struct Pheromone
	{
		PheromoneType type;
		float value;
		Vector2d pos;

		Pheromone(PheromoneType type, float value, Vector2d pos) :
			type(type), value(value), pos(pos) {}
	};

private:
	Vector2d position;
	Vector2d dir;
	Vector2d velocity;
	Vector2d borders;
	float speed;
	float radiusOfView;

	PheromoneType target;


	void chooseDesiredDirection();

	void chooseSpecificDirection();

public:
	void move(const float dt);

	void leavePheromone(float speed, float dt);

	Vector2d getPosition();
};

