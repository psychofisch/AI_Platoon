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
	Kinematics result;
	sf::Vector2f targetDir = agent.getTargetPos() - agent.getPosition();
	float d = magnitude(targetDir);

	//movement
	float estSpeed;
	float	outerRim = agent.getMaxSpeed(),
			innerRim = std::max(agent.getMaxSpeed() / 50.0f, .5f);

	if (d < innerRim) //if success
	{
		if (magnitude(agent.getVelocity()) < 0.1f)
			return Kinematics();

		estSpeed = 0;
	}
	else if (d < outerRim)
		estSpeed = agent.getMaxSpeed()*(d / outerRim);
	else
		estSpeed = agent.getMaxSpeed();

	//std::cout << estSpeed << std::endl;

	result.linearAcc = (normalize(targetDir) * estSpeed) - agent.getVelocity();

	if (magnitude(result.linearAcc) > agent.getMaxAcc())
		result.linearAcc = (result.linearAcc / magnitude(result.linearAcc))*agent.getMaxAcc();

	//rotation
	float targetRot = angleD(targetDir) - agent.getRotation();

	//std::cout << targetRot << std::endl;

	if (targetRot > 1.0f)
		targetRot = 1.0f;
	else if (targetRot < -1.0)
		targetRot = -1.0f;
	else
		targetRot = 0;

	result.angular = targetRot * 200.0f;

	//return
	result.move = true;
	return result;
}

Seek::Seek()
{
}

Seek::~Seek()
{
}

Kinematics Seek::getKinematics(Agent & agent)
{
	Kinematics result;
	Path& p = agent.getPath();

	if (p.size() == 0)
		return result;

	int nwp = p.findNextWaypoint(agent.getPosition());

	sf::Vector2f targetDir = p.getWaypoint(nwp) - agent.getPosition();
	float d = magnitude(targetDir);

	//movement

	//std::cout << estSpeed << std::endl;

	result.linearAcc = (normalize(targetDir) * agent.getMaxSpeed()) - agent.getVelocity();

	if (magnitude(result.linearAcc) > agent.getMaxAcc())
		result.linearAcc = (result.linearAcc / magnitude(result.linearAcc))*agent.getMaxAcc();

	//return
	result.move = true;
	return result;
}
