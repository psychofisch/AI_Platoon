#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>

#include <SFML\System\Vector2.hpp> 

#include "gameobj.h"

class FileIO
{
public:
	FileIO();
	~FileIO();
	
	static sf::Vector2f LoadLevel(const char* path, std::vector<gameobj>& obstacles, std::vector<sf::Texture*>& textures);
	static bool LoadPath(const char* path, std::vector<sf::Vector2f>& waypoints);

	static std::string LoadText(const char*);
};

