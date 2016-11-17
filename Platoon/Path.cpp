#include "Path.h"



Path::Path()
	:m_loop(true),
	m_currentWaypoint(0),
	m_nextWaypoint(0)
{
}


Path::~Path()
{
}

sf::Vector2f Path::getWaypoint(int i) const
{
	if (i >= m_path.size())
	{
		if (m_loop)
			while (i >= m_path.size())
				i -= m_path.size();
		else
			i = m_path.size() - 1;
	}

	return m_path[i];
}

sf::Vector2f Path::getNextWaypoint() const
{
	return m_path[m_nextWaypoint];
}

int Path::getNextWayPointIndex() const
{
	return m_nextWaypoint;
}

int Path::size()
{
	return m_path.size();
}

int Path::addWaypoint(sf::Vector2f p)
{
	m_path.push_back(p);
	return m_path.size();
}

int Path::findNextWaypoint(sf::Vector2f p)
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

	if (closest == m_nextWaypoint)
	{
		m_currentWaypoint = m_nextWaypoint;
		if (m_nextWaypoint == m_path.size() - 1)
		{
			if(m_loop)
				m_nextWaypoint = 0;
		}
		else
		{
			m_nextWaypoint++;
		}
	}

	return m_nextWaypoint;
}
