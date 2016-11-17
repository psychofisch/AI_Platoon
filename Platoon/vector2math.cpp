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
	return radToDeg(angleR(v));
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

sf::Vector2f rotateD(sf::Vector2f v, float angle)
{
	angle = degToRad(angle);
	float length = magnitude(v);
	v = normalize(v);
	float x = (v.x * cosf(angle)) - (v.y * sinf(angle));
	float y = (v.x * sinf(angle)) + (v.y * cosf(angle));
	return sf::Vector2f(x, y) * length;
}
