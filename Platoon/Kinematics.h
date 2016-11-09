#pragma once

#include <SFML\System\Vector2.hpp>

struct Kinematics {
	sf::Vector2f velocity;
	float rotation;

	Kinematics(sf::Vector2f v, float r)
		:velocity(v),
		rotation(r)
	{
	}

	Kinematics()
	{
		Kinematics(sf::Vector2f(0, 0), 0);
	}
};
