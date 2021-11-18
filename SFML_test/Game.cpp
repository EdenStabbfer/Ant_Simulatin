#include "Ant.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Crazy Ants");
	sf::Event event;
	sf::Clock global_clock;
	sf::Clock pheromoneTimer;

	float pherLeaveTime = 500; // in milliseconds

	window.setFramerateLimit(60);

	Ant a1(window);
	sf::CircleShape circle(5);
	circle.setFillColor(sf::Color(0, 255, 0, 255));

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// Update
		float dt = global_clock.restart().asSeconds();
		if (pheromoneTimer.getElapsedTime().asMilliseconds() > pherLeaveTime) {
			pheromoneTimer.restart();

		}

		a1.move(dt);
		circle.setPosition(a1.getPosition().asSFMLVector2f());

		// Draw
		window.clear(sf::Color(0, 0, 0, 0));

		window.draw(circle);

		window.display();
	}

	return 0;
}