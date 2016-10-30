#pragma once
#include <SFML\Graphics.hpp>

#include <sstream>
#include <iostream>

#include "normalize.h"
#include "gameobj.h"
#include "FileIO.h"

/*Colors
41,255,249
38,232,123
77,255,54
187,232,38
255,229,42
*/

class world
{
public:
	world(sf::Vector2f size, sf::RenderWindow* wndw);
	~world();
	void run();
	void setWorldTexture(const char* path);
	int addObstacle(sf::Texture* texture, sf::Vector2f position);
	bool loadObstacles(const char* path);
private:
	sf::RenderWindow* m_window;
	sf::Texture m_worldTexture;
	sf::Sprite m_worldSprite;
	sf::View m_camera;
	sf::Vector2f m_size;
	sf::Text m_debugText;
	sf::Font m_font;
	std::vector<gameobj> m_obstacles;
	std::vector<sf::Texture> m_textures;
};

