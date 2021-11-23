#pragma once

#include <iostream>
#include <random>

#include <SFML/Graphics.hpp>

class Vector2d
{
public:
	float x, y;

	Vector2d(float xCord = 1.f, float yCord = 1.f);
	
	friend Vector2d operator+(const Vector2d& v1, const Vector2d& v2);

	friend Vector2d operator-(const Vector2d& v1, const Vector2d& v2);

	friend Vector2d operator*(const Vector2d& v1, const Vector2d& v2);
	friend Vector2d operator*(const Vector2d& v, float value);
	friend Vector2d operator*(float value, const Vector2d& v);

	friend Vector2d operator/(const Vector2d& v1, const Vector2d& v2);
	friend Vector2d operator/(const Vector2d& v, float value);
	friend Vector2d operator/(float value, const Vector2d& v);

	Vector2d operator+=(const Vector2d& v);
	Vector2d operator-=(const Vector2d& v);
	Vector2d operator*=(float value);

	friend std::ostream& operator<< (std::ostream& out, const Vector2d& v);

	float sqLength();
	float length();
	float distanceTo(const Vector2d& v);
	float distanceTo(float x, float y);
	Vector2d VectorTo(const Vector2d& v);
	Vector2d VectorTo(float x, float y);
	Vector2d& normalize();
	float dot(Vector2d& v);
	sf::Vector2f asSFMLVector2f();
	Vector2d copy();
};

Vector2d randomVectorInCircle();
