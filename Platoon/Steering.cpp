#include "Steering.h"

#include "Agent.h"

Steering::Steering()
{
}


Steering::~Steering()
{
}

float Steering::rotate(float r)
{
	if (r > 360.f)
		r -= 360.f;
	else if (r < 0.f)
		r += 360.f;

	if (r > 181.f)
		r = 1.0f;
	else if (r < 179.f)
		r = -1.0f;
	else
		r = 0;

	r *= 200.0f;

	//return
	return r;
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
	float targetRot = angleD(targetDir) - (agent.getRotation() - 180.f);

	result.angular = rotate(targetRot);

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

	//rotation
	float targetRot = angleD(targetDir) - (agent.getRotation() - 180.f);
	result.angular = rotate(targetRot);

	//return
	result.move = true;
	return result;
}

ObstacleAvoid::ObstacleAvoid()
{
}

ObstacleAvoid::~ObstacleAvoid()
{
}

Kinematics ObstacleAvoid::getKinematics(Agent & agent)
{
	Kinematics result;
	std::vector<gameobj> obst = *agent.getObstacles();
	int collisionInd = -1;
	sf::Vector2f w[3];
	agent.getWhiskers(w);
	for (int i = 0; i < obst.size(); ++i)
	{
		if (obst[i].sprite.getGlobalBounds().contains(agent.getPosition() + w[1]))
		{
			collisionInd = i;
			//std::cout << "collision!\n";
			break;
		}
	}

	if (collisionInd == -1)
		return result;

	sf::Vector2f realCorners[4];
	getRealCorners(obst[collisionInd].sprite, realCorners);

	agent.m_stepsSprite.setColor(sf::Color::Magenta);
	sf::Vector2f intersection(100.f, 100.f);
	int intersectionIndex = -1;
	for (int i = 0; i < 4; ++i)
	{
		sf::Vector2f tmp;
		int j = i + 1;
		if (i == 3)
			j = 0;
		
		tmp = lineIntersection(sf::Vector2fLines(agent.getPosition(), agent.getPosition() + w[1]), sf::Vector2fLines(realCorners[i], realCorners[j]));

		if (magnitude(tmp - agent.getPosition()) < magnitude(intersection - agent.getPosition())
				&& isBetween(realCorners[j], tmp, realCorners[i]))
		{
			intersection = tmp;
			intersectionIndex = i;
		}

		//std::cout << intersection.x << ":" << intersection.y << std::endl;
	}

	if (!obst[collisionInd].sprite.getGlobalBounds().contains(intersection) || magnitude(intersection - agent.getPosition()) > magnitude(w[1]))
		return result;

	agent.m_stepsSprite.setPosition(intersection);
	agent.m_wndw->draw(agent.m_stepsSprite);

	sf::Vector2f normal1(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].y - realCorners[intersectionIndex].y, -(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].x - realCorners[intersectionIndex].x));
	sf::Vector2f normal2(-normal1.x, -normal1.y);

	//if (magnitude(intersection - agent.getPosition() - normal1) < magnitude(intersection - agent.getPosition() - normal2))
	//	normal1 = normal2;

	agent.m_stepsSprite.setPosition(normal1);
	agent.m_wndw->draw(agent.m_stepsSprite);

	normal1 = normalize(normal1);

	result.linearAcc = normal1 * agent.getMaxAcc();
	result.move = true;
	return result;
}
