#pragma once

#include "Kinematics.h"
#include "vector2math.h"

#include <iostream>

class Agent;

class Steering
{
public:
	Steering();
	~Steering();

	virtual Kinematics getKinematics(Agent& agent) = 0;
};

class Arrive : public Steering {
public:
	Arrive();
	~Arrive();

	Kinematics getKinematics(Agent& agent);
};

class Seek : public Steering {
public:
	Seek();
	~Seek();

	Kinematics getKinematics(Agent& agent);
};
