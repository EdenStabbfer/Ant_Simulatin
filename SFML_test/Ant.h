#pragma once

#include "Vector2d.h"
#include "Config.h"


class Ant
{
public:
	Ant(float maxSpeed = 45.f);

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

		Pheromone(PheromoneType p_type, float p_value, float p_x, float p_y) :
			type(p_type), value(p_value), x(p_x), y(p_y){}
	};

private:
	Vector2d position;
	Vector2d desiredDir;
	Vector2d velocity;
	Vector2d borders;
	float speed;

	PheromoneType target;

	void chooseDesiredDirection();
	void leavePheromone(std::vector<Pheromone>& ph);
	bool findHomeFood(const Vector2d& foodPos, const Vector2d& homePos);

public:
	void update(std::vector<Pheromone>& ph, const float& dt, const float& pherDt, const Vector2d& foodPos, const Vector2d& homePos);
	void move(const float& dt);
	PheromoneType getTarget();

	Vector2d& getPosition();
	Vector2d& getVelocity();
	Vector2d& getDesiredDir();
};

