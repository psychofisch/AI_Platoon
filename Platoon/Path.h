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
	sf::Vector2f getCurrentWaypoint() const;

	int addWaypoint(sf::Vector2f p);
	int findClosestWaypoint(sf::Vector2f p);

private:
	std::vector<sf::Vector2f> m_path;
	int m_currentWaypoint;
};

