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

void getRealCorners(const sf::Sprite & sprite, sf::Vector2f* out)
{
	sf::FloatRect srect = sprite.getLocalBounds();

	out[0] = sprite.getTransform().transformPoint(srect.left, srect.top);
	out[1] = sprite.getTransform().transformPoint(srect.width, srect.top);
	out[2] = sprite.getTransform().transformPoint(srect.width, srect.height);
	out[3] = sprite.getTransform().transformPoint(srect.left, srect.height);
}

sf::Vector2f lineIntersection(sf::Vector2fLines l1, sf::Vector2fLines l2)
{
	//todo: http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	sf::Vector2f intersection;

	intersection.x = ((((l1.p1.x*l1.p2.y) - (l1.p1.y*l1.p2.x))*(l2.p1.x - l2.p2.x)) - ((l1.p1.x - l1.p2.x)*((l2.p1.x*l2.p2.y) - (l2.p1.y*l2.p2.x))))
						/ (((l1.p1.x - l1.p2.x)*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*(l2.p1.x - l2.p2.x)));

	intersection.y = ((((l1.p1.x*l1.p2.y) - (l1.p1.y*l1.p2.x))*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*((l2.p1.x*l2.p2.y) - (l2.p1.y*l2.p2.x))))
						/ (((l1.p1.x - l1.p2.x)*(l2.p1.y - l2.p2.y)) - ((l1.p1.y - l1.p2.y)*(l2.p1.x - l2.p2.x)));

	return intersection;
}

float cross2D(sf::Vector2f a, sf::Vector2f b)
{
	return (a.x*b.y)-(a.y*b.x);
}

bool isBetween(sf::Vector2f a, sf::Vector2f p, sf::Vector2f b)
{
	if (magnitude(p - a) < magnitude(b - a))
		return true;
	else
		return false;
}

bool isNotZero(sf::Vector2f & v)
{
	if (v.x != 0.0f && v.y != 0.0f)
		return true;
	else
		return false;
}

sf::FloatRect growBox(sf::FloatRect & r, float f)
{
	return sf::FloatRect(r.left - f, r.top - f, r.width + f, r.height + f);
}
