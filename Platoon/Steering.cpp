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
	}

	if (!growBox(s.getGlobalBounds(), 10.0f).contains(intersection) || magnitude(intersection - a.getPosition()) > magnitude(w))
		return sf::Vector2f(0.f, 0.f);

	sf::Vector2f normal1(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].y - realCorners[intersectionIndex].y, -(realCorners[(intersectionIndex == 3) ? 0 : intersectionIndex + 1].x - realCorners[intersectionIndex].x));
	sf::Vector2f normal2(-normal1.x, -normal1.y);

	return normalize(normal1);
}

Separation::Separation()
{
}

Separation::~Separation()
{
}

Kinematics Separation::getKinematics(Agent & agent)
{
	Kinematics result;
	std::vector<Agent>* others = agent.getOtherAgents();
	if (others == nullptr)
		return result;

	for (auto&& a : *others)
	{
		if (&a == &agent)
			continue;
		float e = 60.0f;
		sf::Vector2f space = agent.getPosition() - a.getPosition();
		float dist = magnitude(space);
		if (dist > e)
			continue;

		result.move = true;
		float factor = (e - dist) / e;
		result.linearAcc += factor * space;
	}

	if (magnitude(result.linearAcc) > 1.0f)
		result.linearAcc = normalize(result.linearAcc);
	result.linearAcc *= agent.getMaxAcc();
	return result;
}

CollisionAvoid::CollisionAvoid()
{
}

CollisionAvoid::~CollisionAvoid()
{
}

Kinematics CollisionAvoid::getKinematics(Agent & agent)
{
	Kinematics result;
	std::vector<Agent>* enemies = agent.getEnemies();

	if (enemies == nullptr)
		return result;

	sf::Vector2f enemy_pos(INFINITY, INFINITY);
	bool found = false;
	for (auto&& e : *enemies)
	{
		sf::Vector2f tmp = agent.getPosition() - e.getPosition();
		if (magnitude(tmp) > 400.f || magnitude(tmp) > magnitude(enemy_pos))
			continue;
		float angle = angleD(normalize(tmp)) - angleD(normalize(agent.getVelocity()));
		if (isBetween(120.f, fabsf(angle), 220.f))
		{
			enemy_pos = tmp;
			found = true;
		}
	}

	if (!found)
		return result;

	float d = magnitude(enemy_pos);

	//movement
	result.linearAcc = (normalize(enemy_pos) * agent.getMaxAcc());

	if (magnitude(result.linearAcc) > agent.getMaxAcc())
		result.linearAcc = normalize(result.linearAcc)*agent.getMaxAcc();

	//rotation
	float targetRot = angleD(enemy_pos) - (agent.getRotation() - 180.f);
	result.angular = rotate(targetRot);

	//return
	result.move = true;
	return result;
}
