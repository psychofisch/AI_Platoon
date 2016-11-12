#pragma once

#include <SFML\System\Vector2.hpp>

struct Kinematics {
	bool enabled;
	sf::Vector2f velocity;
	float rotation;

	Kinematics(sf::Vector2f v, float r, bool e)
		:velocity(v),
		rotation(r),
		enabled(e)
	{
	}

	Kinematics() : Kinematics(sf::Vector2f(0, 0), 0, false)
	{
	}
};
