#include "Path.h"



Path::Path()
{
}


Path::~Path()
{
}

sf::Vector2f Path::getWaypoint(int i) const
{
	return m_path[i];
}

sf::Vector2f Path::getCurrentWaypoint() const
{
	return m_path[m_currentWaypoint];
}

int Path::addWaypoint(sf::Vector2f p)
{
	m_path.push_back(p);
	return m_path.size();
}

int Path::findClosestWaypoint(sf::Vector2f p)
{
	int closest = -1;
	float dist = INFINITY;
	for (int i = 0; i < m_path.size(); ++i)
	{
		float dist_tmp = magnitude(p - m_path[i]);
		if (dist_tmp < dist)
		{
			dist = dist_tmp;
			closest = i;
		}
	}

	return closest;
}
