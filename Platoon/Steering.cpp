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
	int collisionInd[3] = { -1 };
	sf::Vector2f w[3];
	agent.getWhiskers(w);
	for (int i = 0; i < 3; ++i)
	{
		for (int o = 0; o < obst.size(); ++o)
		{
			if (obst[o].sprite.getGlobalBounds().contains(agent.getPosition() + w[i]))
			{
				collisionInd[i] = o;
				//std::cout << "collision!\n";
				break;
			}
		}

		if(collisionInd[i] != -1)
			result.linearAcc += i_collides(agent, obst[collisionInd[i]].sprite, w[i]);
	}

	if (!isNotZero(result.linearAcc))
		return Kinematics();
	result.linearAcc = normalize(result.linearAcc) * agent.getMaxAcc();
	result.move = true;
	return result;
}

sf::Vector2f ObstacleAvoid::i_collides(Agent& a, sf::Sprite& s, sf::Vector2f w)
{
	sf::Vector2f realCorners[4];
	getRealCorners(s, realCorners);

	a.m_stepsSprite.setColor(sf::Color::Magenta);
	sf::Vector2f intersection(100.f, 100.f);
	int intersectionIndex = -1;
	for (int i = 0; i < 4; ++i)
	{
		sf::Vector2f tmp;
		int j = i + 1;
		if (i == 3)
			j = 0;

		tmp = lineIntersection(sf::Vector2fLines(a.getPosition(), a.getPosition() + w), sf::Vector2fLines(realCorners[i], realCorners[j]));

		if (magnitude(tmp - a.getPosition()) < magnitude(intersection - a.getPosition())
			&& isBetween(realCorners[j], tmp, realCorners[i]))
		{
			intersection = tmp;
			intersectionIndex = i;
		}

		//std::cout << intersection.x << ":" << intersection.y << std::endl;
	}

	if (!s.getGlobalBounds().contains(intersection) || magnitude(intersection - a.getPosition()) > magnitude(w))
		return sf::Vector2f(0.f, 0.f);

	a.m_stepsSprite.setPosition(intersection);
	a.m_wndw->draw(a.m_stepsSprite);

	sf::Vector2f normal1(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].y - realCorners[intersectionIndex].y, -(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].x - realCorners[intersectionIndex].x));
	sf::Vector2f normal2(-normal1.x, -normal1.y);

	//if (magnitude(intersection - agent.getPosition() - normal1) < magnitude(intersection - agent.getPosition() - normal2))
	//	normal1 = normal2;

	a.m_stepsSprite.setPosition(normal1);
	a.m_wndw->draw(a.m_stepsSprite);

	return normalize(normal1);
}
