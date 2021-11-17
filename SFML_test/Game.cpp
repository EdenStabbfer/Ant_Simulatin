#include "Ant.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Crazy Ants");
	sf::Event event;
	sf::Clock clock;

	window.setFramerateLimit(60);

	Ant a1(Vector2d(window.getSize().x / 2, window.getSize().y / 2));
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
		float dt = clock.restart().asSeconds();
		a1.move(dt);
		circle.setPosition(a1.getPosition().asSFMLVector2f());

		// Draw
		window.clear(sf::Color(0, 0, 0, 0));

		window.draw(circle);

		window.display();
	}

	return 0;
}