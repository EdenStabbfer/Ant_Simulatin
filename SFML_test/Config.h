#pragma once

namespace Config
{
	// Game
	const int width = 1280;
	const int height = 720;

	// Time
	const float phUpdateTime = 0.33; // in seconds
	const int frameRate = 60;

	// Ant
	const float wonderStrength(12.f);
	const float streeringStrength(8.f);
	const float radiusOfView(20.f);
	const float antSize(10.f);

	// Pheromones
	const float pheromoneEvaporationRate(0.001);
	const float pheromoneUpdateRate(0.01);
	const int distanceStrength(5);
	const int pheromoneStrength(3);
}