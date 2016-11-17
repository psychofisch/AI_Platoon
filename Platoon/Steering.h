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

	float rotate(float r);
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

class ObstacleAvoid : public Steering {
public:
	ObstacleAvoid();
	~ObstacleAvoid();

	// Inherited via Steering
	virtual Kinematics getKinematics(Agent & agent) override;
};
