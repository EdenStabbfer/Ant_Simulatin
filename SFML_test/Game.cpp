#include "Ant.h"


int main()
{
	// C????????
	sf::RenderWindow window(sf::VideoMode(Config::width, Config::height), "Crazy Ants");
	sf::Event event;
	sf::Clock global_clock;
	sf::Clock pheromoneTimer;

	window.setFramerateLimit(Config::frameRate);

	// ???????? ????????
	std::vector<Ant::Pheromone> pheromones;
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> borders;
	Ant ants[Config::antNumber];
	for (auto& a : ants)
		a = Ant(&pheromones, &borders, Config::maxSpeed);

	// ??????
	sf::CircleShape antShape(3);
	antShape.setOrigin(3, 3);
	antShape.setFillColor(sf::Color(20, 210, 60));

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

	bool startLine(false);

	// ??????? ????
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
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					if (!startLine)
					{
						startLine = true;
						borders.emplace_back(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
					}
					else
						startLine = false;
				}
			}
		}

		if (startLine)
			borders.back().first = sf::Vector2f(sf::Mouse::getPosition(window));


		// Time Update
		float dt = global_clock.restart().asSeconds();

		// Ants Update
		for (Ant& a : ants)
		{
			a.update(dt, pheromoneTimer.getElapsedTime().asSeconds(), food, home);
		}

		// Pheromone evaporation
		if (pheromoneTimer.getElapsedTime().asSeconds() > Config::phUpdateTime)
			pheromoneTimer.restart();

		// draw
		window.clear(sf::Color(0, 0, 0));

		for (auto& ph : pheromones)
		{
			sf::CircleShape c(1);
			c.setPosition(ph.position.asSFMLVector2f());
			if (ph.type == Ant::TO_HOME)
				c.setFillColor(sf::Color(0, 0, 255, static_cast<int>(min(ph.value, 1.f) * 255)));
			else
				c.setFillColor(sf::Color(255, 0, 0, static_cast<int>(min(ph.value, 1.f) * 255)));
			window.draw(c);
			ph.value *= (1.f - Config::pheromoneEvaporationRate);
			auto iter = std::find_if(pheromones.begin(), pheromones.end(), [&](const auto& ph) {return ph.value < Config::pheromoneMinValue; });
			if (iter != pheromones.end())
				pheromones.erase(iter);
		}
		for (Ant& a : ants)
		{
			/*sf::Vertex line[] =
			{
				sf::Vertex(a.getPosition().asSFMLVector2f()),
				sf::Vertex(a.getPosition().asSFMLVector2f() + a.getVelocity().asSFMLVector2f())
			};
			window.draw(line, 2, sf::Lines);*/
			antShape.setPosition(a.getPosition().asSFMLVector2f());
			window.draw(antShape);
		}
		for (auto& pair : borders)
		{
			sf::Vertex line[]{sf::Vertex(pair.first), sf::Vertex(pair.second)};
			window.draw(line, 3, sf::Lines);
		}
		window.draw(foodShape);
		window.draw(homeShape);

		window.display();
	}

	return 0;
}