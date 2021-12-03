#include "Ant.h"

float min(float a, float b)
{
	return (a <= b) ? a : b;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(Config::width, Config::height), "Crazy Ants");
	sf::Event event;
	sf::Clock global_clock;
	sf::Clock pheromoneTimer;

	window.setFramerateLimit(Config::frameRate);

	std::vector<Ant::Pheromone> pheromones;
	Ant ants[10];

	Vector2d food(100.f, 100.f);
	sf::CircleShape foodShape(20);
	foodShape.setOrigin(20, 20);
	foodShape.setFillColor(sf::Color(128, 0, 128));
	foodShape.setPosition(food.asSFMLVector2f());

	Vector2d home(window.getSize().x / 2, window.getSize().y / 2);
	sf::CircleShape homeShape(20);
	homeShape.setOrigin(20, 20);
	homeShape.setFillColor(sf::Color(255, 0, 0));
	homeShape.setPosition(home.asSFMLVector2f());

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					food.x = event.mouseButton.x;
					food.y = event.mouseButton.y;
					foodShape.setPosition(event.mouseButton.x, event.mouseButton.y);
				}
			}
		}
		// Time Update
		float dt = global_clock.restart().asSeconds();

		// Ants Update
		for (Ant& a : ants)
		{
			a.update(pheromones, dt, pheromoneTimer.getElapsedTime().asSeconds(), food, home);
		}

		// Pheromone evaporation
		if (pheromoneTimer.getElapsedTime().asSeconds() > Config::phUpdateTime)
			pheromoneTimer.restart();

		// Draw
		window.clear(sf::Color(0, 0, 0));

		for (auto& ph : pheromones)
		{
			sf::CircleShape c(1);
			c.setPosition(ph.x, ph.y);
			if (ph.type == Ant::TO_HOME)
				c.setFillColor(sf::Color(0, 0, 255, static_cast<int>(min(ph.value, 1.f)*255)));
			else
				c.setFillColor(sf::Color(255, 0, 0, static_cast<int>(min(ph.value, 1.f) * 255)));
			window.draw(c);
			ph.value *= 1 - Config::pheromoneEvaporationRate;
		}
		for (Ant& a : ants)
		{
			sf::Vertex line[] =
			{
				sf::Vertex(a.getPosition().asSFMLVector2f()),
				sf::Vertex(a.getPosition().asSFMLVector2f() + a.getVelocity().asSFMLVector2f())
			};
			window.draw(line, 2, sf::Lines);
			sf::CircleShape circle(5);
			circle.setOrigin(5, 5);
			circle.setPosition(a.getPosition().asSFMLVector2f());
			circle.setFillColor(sf::Color(0, 255, 0, 255));
			window.draw(circle);
		}
		window.draw(foodShape);
		window.draw(homeShape);

		window.display();
	}

	return 0;
}