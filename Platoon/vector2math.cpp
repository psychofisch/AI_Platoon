#pragma once
#include "vector2math.h"

sf::Vector2f normalize(const sf::Vector2f& source)
{
	float length = magnitude(source);
	if (length != 0)
		return sf::Vector2f(source.x / length, source.y / length);
	else
		return source;
}

float magnitude(const sf::Vector2f& source)
{
	return sqrt((source.x * source.x) + (source.y * source.y));
}

sf::Vector2f multiply(const sf::Vector2f & left, const sf::Vector2f & right)
{
	return sf::Vector2f(left.x*right.x, left.y*right.y);
}

float angleD(const sf::Vector2f & v)
{
	return angleR(v)*(180 / sf::PI);
}

float angleR(const sf::Vector2f & v)
{
	return atan2(v.y, v.x);
}

float radToDeg(float Rad)
{
	return Rad / sf::PI * 180.f;
}

float degToRad(float d)
{
	return (d / 180.f) * sf::PI;
}
