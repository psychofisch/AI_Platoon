#pragma once

#include "Kinematics.h"

class Agent;

class Steering
{
public:
	Steering();
	~Steering();

	virtual Kinematics getKinematics(Agent& agent, sf::Vector2f targetPos) = 0;
};

class Arrive : public Steering {
public:
	Arrive();
	~Arrive();

	Kinematics getKinematics(Agent& agent, sf::Vector2f targetPos);
};
