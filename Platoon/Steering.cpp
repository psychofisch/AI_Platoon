#include "Steering.h"

#include "Agent.h"

Steering::Steering()
{
}


Steering::~Steering()
{
}

Arrive::Arrive()
{
}

Arrive::~Arrive()
{
}

Kinematics Arrive::getKinematics(Agent & agent, sf::Vector2f targetPos)
{
	sf::Vector2f targetDir = targetDir - agent.getPosition();
	return Kinematics(targetDir * agent.getMaxSpeed(), 0);
}
