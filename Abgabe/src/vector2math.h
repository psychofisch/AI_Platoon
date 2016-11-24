#pragma once

#include <SFML\Graphics.hpp>

namespace sf {
	const float PI = 3.1415f;

	struct Vector2fLines {
		sf::Vector2f p1;
		sf::Vector2f p2;

		Vector2fLines(sf::Vector2f a, sf::Vector2f b)
			:p1(a),
			p2(b)
		{}
	};
}

sf::Vector2f normalize(const sf::Vector2f&);
float magnitude(const sf::Vector2f&);
sf::Vector2f multiply(const sf::Vector2f& left, const sf::Vector2f& right);
float dot(const sf::Vector2f& left, const sf::Vector2f& right);
float angleD(const sf::Vector2f& v);
float angleR(const sf::Vector2f& v);
float radToDeg(float r);
float degToRad(float d);
sf::Vector2f rotateD(sf::Vector2f v, float aD);
void getRealCorners(const sf::Sprite& sprite, sf::Vector2f* out);//out -> sf::Vector2f[4]
sf::Vector2f lineIntersection(sf::Vector2fLines l1, sf::Vector2fLines l2);
float cross2D(sf::Vector2f a, sf::Vector2f b);
bool isBetween(sf::Vector2f a, sf::Vector2f p, sf::Vector2f b);
bool isBetween(float a, float p, float b);
bool isNotZero(sf::Vector2f& v);
sf::FloatRect growBox(sf::FloatRect& r, float f);
