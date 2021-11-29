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
	const float wonderStrength(8.f);
	const float streeringStrength(10.f);
	const float radiusOfView(50.f);
	const float antSize(8.f);

	// Pheromones
	const float pheromoneEvaporationRate(0.001);
	const float pheromoneUpdateRate(0.01);
	const int distanceStrength(3);
	const int pheromoneStrength(5);
}