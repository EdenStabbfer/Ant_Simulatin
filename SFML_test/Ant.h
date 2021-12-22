#pragma once

#include "Vector2d.h"
#include "Math.h"
#include "Config.h"


class Ant
{
public:
	enum PheromoneType
	{
		TO_HOME,
		TO_FOOD
	};

	struct Pheromone
	{
		PheromoneType type;
		float value;
		Vector2d position;

		Pheromone(PheromoneType p_type, float p_value, Vector2d p_position) :
			type(p_type), value(p_value), position(p_position){}
	};

private:
	Vector2d position;
	Vector2d desiredDir;
	Vector2d velocity;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* borders;

	float speed;
	float coveredDistance;

	PheromoneType target;

	std::vector<Pheromone>* pheromones;

	void chooseDesiredDirection();
	void leavePheromone();
	bool findHomeFood(const Vector2d& foodPos, const Vector2d& homePos);

public:
	Ant(std::vector<Ant::Pheromone>* pheromones = nullptr, std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* borders = nullptr, float maxSpeed = 1);

	void update(const float& dt, const float& pherDt, const Vector2d& foodPos, const Vector2d& homePos);
	void move(const float& dt);
	PheromoneType getTarget();

	Vector2d& getPosition();
	Vector2d& getVelocity();
	Vector2d& getDesiredDir();
};

