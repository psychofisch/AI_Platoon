#pragma once

#include "Kinematics.h"
#include "Steering.h"
#include "gameobj.h"

class Agent : public gameobj
{
public:
	enum SteerMode { STEER_ARRIVE };

	Agent();
	Agent(int mode);
	~Agent();

	float getMaxSpeed();
	
	void moveTo(sf::Vector2f target);
	bool setSteering(int mode);//accepts SteerMode; int for compatibility
	bool setMaxSpeed(float newMaxSpeed);//accepts Speeds > 0.0f; returns True if value is accepted
	void setSprite(std::vector<sf::Texture*>& textures, const char * path);

private:
	float m_maxSpeed;
	Steering* m_steering;
};

