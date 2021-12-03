#pragma once

namespace Config
{
	// Game
	const int width = 1280;
	const int height = 720;

	// Time
	const float phUpdateTime = 0.5; // in seconds
	const int frameRate = 30;

	// Ant
	const float wonderStrength(.2f);
	const float streeringStrength(90.f);
	const float radiusOfView(40.f);
	const float antSize(8.f);

	// Pheromones
	const float pheromoneEvaporationRate(0.0005);
	const float pheromoneUpdateRate(0.09);
	const int distanceStrength(2);
	const int pheromoneStrength(5);
}