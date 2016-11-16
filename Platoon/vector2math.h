#pragma once

#include <SFML\Graphics.hpp>

sf::Vector2f normalize(const sf::Vector2f&);
float magnitude(const sf::Vector2f&);
sf::Vector2f multiply(const sf::Vector2f& left, const sf::Vector2f& right);
float angleD(const sf::Vector2f& v);
float angleR(const sf::Vector2f& v);
float radToDeg(float r);
float degToRad(float d);

namespace sf {
	const float PI = 3.1415f;
}
