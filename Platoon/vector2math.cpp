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
