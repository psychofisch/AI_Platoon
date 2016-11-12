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

Kinematics Arrive::getKinematics(Agent & agent)
{
	sf::Vector2f targetDir = agent.getTargetPos() - agent.getPosition();

	if (magnitude(targetDir) < std::max(agent.getMaxSpeed()/50.0f, .5f))
		return Kinematics();

	targetDir = normalize(targetDir);
	float targetRot = (atan2(targetDir.y, targetDir.x)*(180 / sf::PI)) - agent.getRotation();

	std::cout << agent.getRotation() << std::endl;

	if (targetRot > 1.0f)
		targetRot = 1.0f;
	else if (targetRot < -1.0)
		targetRot = -1.0f;
	else
		targetRot = 0;

	return Kinematics(targetDir * agent.getMaxSpeed(), targetRot * 200.0f, true);
}
