#include "Ant.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Crazy Ants");
	sf::Event event;
	sf::Clock global_clock;
	sf::Clock pheromoneTimer;

	float pherLeaveTime = 0.5; // in seconds

	window.setFramerateLimit(60);

	Ant a1(window);
	sf::CircleShape circle(5);
	circle.setFillColor(sf::Color(0, 255, 0, 255));

	std::vector<Ant::Pheromone> pheromones;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// Update
		float dt = global_clock.restart().asSeconds();

		a1.move(pheromones, dt, pheromoneTimer, pherLeaveTime);
		circle.setPosition(a1.getPosition().asSFMLVector2f());

		// Draw
		window.clear(sf::Color(0, 0, 0, 0));

		window.draw(circle);
		for (auto& ph : pheromones)
		{
			sf::CircleShape c(1);
			c.setFillColor(sf::Color(0, 0, 255, 255));
			c.setPosition(ph.x, ph.y);
			window.draw(c);
		}

		window.display();
	}

	return 0;
}