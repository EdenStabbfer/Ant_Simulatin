#include <cmath>

#include "Vector2d.h"



Vector2d::Vector2d(float xCord, float yCord)
{
	this->x = xCord;
	this->y = yCord;
}

Vector2d operator+(const Vector2d& v1, const Vector2d& v2)
{
	return Vector2d(v1.x + v2.x, v1.y + v2.y);
}

Vector2d operator-(const Vector2d& v1, const Vector2d& v2)
{
	return Vector2d(v1.x - v2.x, v1.y - v2.y);
}

Vector2d operator*(const Vector2d& v1, const Vector2d& v2)
{
	return Vector2d(v1.x * v2.x, v1.y * v2.y);
}

Vector2d operator*(const Vector2d& v, float value)
{
	return Vector2d(v.x * value, v.y * value);
}

Vector2d operator*(float value, const Vector2d& v)
{
	return Vector2d(v.x * value, v.y * value);
}

Vector2d operator/(const Vector2d& v1, const Vector2d& v2)
{
	return Vector2d(v1.x / v2.x, v1.y / v2.y);
}

Vector2d operator/(const Vector2d& v, float value)
{
	return Vector2d(v.x / value, v.y / value);
}

Vector2d operator/(float value, const Vector2d& v)
{
	return Vector2d(v.x / value, v.y / value);
}

Vector2d Vector2d::operator+=(const Vector2d& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2d Vector2d::operator-=(const Vector2d& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

Vector2d Vector2d::operator*=(float value)
{
	this->x *= value;
	this->y *= value;
	return *this;
}

float Vector2d::sqLength()
{
	return (this->x * this->x) + (this->y * this->y);
}

float Vector2d::length()
{
	return sqrt(sqLength());
}

float Vector2d::distanceTo(Vector2d& v)
{
	return sqrt(pow(v.x - this->x, 2) + pow(v.y - this->y, 2));
}

float Vector2d::distanceTo(float x, float y)
{
	return sqrt(pow(x - this->x, 2) + pow(y - this->y, 2));
}

Vector2d Vector2d::VectorTo(float x, float y)
{
	return Vector2d(x - this->x, y - this->y);
}

Vector2d& Vector2d::normalize()
{
	float len = this->length();
	this->x /= len;
	this->y /= len;
	return *this;
}

float Vector2d::dot(Vector2d& v)
{
	return this->x * v.x + this->y * v.y;
}

sf::Vector2f Vector2d::asSFMLVector2f()
{
	return sf::Vector2f(this->x, this->y);
}

std::ostream& operator<<(std::ostream& out, const Vector2d& v)
{
	return out << "Vector(" << v.x << ", " << v.y << ")";
}

Vector2d Vector2d::copy()
{
	return Vector2d(x, y);
}

// Random vector in circle
Vector2d randomVectorInCircle()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> uid(-1, 1);
	return Vector2d(uid(gen), uid(gen)).normalize();
}

