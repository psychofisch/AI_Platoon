#pragma once

#include <vector>

#include <SFML\System\Vector2.hpp>

#include "vector2math.h"

class Path
{
public:
	Path();
	~Path();

	sf::Vector2f getWaypoint(int i) const;
	sf::Vector2f getNextWaypoint() const;
	int getNextWayPointIndex() const;
	int size();

	int addWaypoint(sf::Vector2f p);
	int findNextWaypoint(sf::Vector2f p);

private:
	bool m_loop;
	int m_currentWaypoint, m_nextWaypoint;
	std::vector<sf::Vector2f> m_path;
};

