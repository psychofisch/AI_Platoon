#pragma once
#include <SFML\Graphics.hpp>

#include <sstream>

#include "normalize.h"

class world
{
public:
	world(sf::Vector2f size, sf::RenderWindow* wndw);
	~world();
	void run();
	void setWorldTexture(const char* path);
private:
	sf::RenderWindow* m_window;
	sf::Texture m_worldTexture;
	sf::Sprite m_worldSprite;
	sf::View m_camera;
	sf::Vector2f m_size;
	sf::Text m_debugText;
	sf::Font m_font;
};

