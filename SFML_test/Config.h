#pragma once

namespace Config
{
	// Game
	const int width = 1280;
	const int height = 720;

	// Time
	const float phUpdateTime = 0.5f; // in seconds
	const int frameRate = 30;

	// Ant
	const int antNumber = 200;
	const float maxSpeed = 40.f;
	const float wonderStrength(.2f);
	const float streeringStrength(150.f);
	const float radiusOfView(40.f);
	const float sqRadiusOfView = radiusOfView * radiusOfView;
	const float antSize(5.f);
	const float sqAntSize = antSize * antSize;
	const float visionAngle(70 * 3.1415f / 180.f); // first number is angle in degrees
	const bool turnOnTarget(false);

	// Pheromones
	const float pheromoneMinValue = 0.07f;
	const float pheromoneEvaporationRate(0.0009f);
	const float pheromoneLeaveRate = 200.f;
	const float pheromoneUpdateRate(10.f);
	const int distanceStrength(5);
	const int pheromoneStrength(3);

	//const int nSectorsX = 10;
	//const int nSectorsY = 6;
}