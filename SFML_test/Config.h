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
	const float wonderStrength(.2f);
	const float streeringStrength(90.f);
	const float radiusOfView(40.f);
	const float antSize(5.f);

	// Pheromones
	const float pheromoneMinValue = 0.05f;
	const float pheromoneEvaporationRate(0.0001f);
	const float pheromoneLeaveRate = 130.f;
	const float pheromoneUpdateRate(20.f);
	const int distanceStrength(2);
	const int pheromoneStrength(5);
}