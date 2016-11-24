#pragma once

#include <SFML\System\Vector2.hpp>

struct Kinematics {
	bool move;
	sf::Vector2f linearAcc;
	float angular;

	Kinematics(sf::Vector2f v, float r, bool e)
		:linearAcc(v),
		angular(r),
		move(e)
	{
	}

	Kinematics() : Kinematics(sf::Vector2f(0, 0), 0, false)
	{
	}
};
