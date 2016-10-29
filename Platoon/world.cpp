#include "world.h"



world::world(sf::Vector2f size, sf::RenderWindow* wndw)
	:m_window(wndw),
	m_size(size)
{
	m_font.loadFromFile("NotoSans-Regular.ttf");
	m_debugText.setFont(m_font);
	m_debugText.setScale(sf::Vector2f(0.6f, 0.6f));
	m_debugText.setPosition(m_window->getSize().x - 150.0f, 20.0f);
	m_debugText.setString("DEBUG");
	m_debugText.setColor(sf::Color::Green);
	m_debugText.setOutlineThickness(1.0f);
	m_debugText.setOutlineColor(sf::Color::Black);
}


world::~world()
{
}

void world::run()
{
	int mouseMoveRectSize = 400;
	sf::IntRect mouseMoveRect = sf::IntRect((m_window->getSize().x - mouseMoveRectSize) / 2, (m_window->getSize().x - mouseMoveRectSize) / 2, mouseMoveRectSize, mouseMoveRectSize);
	sf::Vector2f windowCenter(m_window->getSize());
	windowCenter /= 2.0f;
	
	bool quit = false;
	while (!quit)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*m_window);
		sf::Vector2f mousePos_mapped = m_window->mapPixelToCoords(mousePos, m_camera);
		
		//controls
		sf::Event eve;
		while (m_window->pollEvent(eve))
		{
			if (eve.type == sf::Event::Closed)
			{
				quit = true;
				break;
			}

			if (eve.type == sf::Event::MouseButtonPressed)
			{
				break;
			}

			if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				case sf::Keyboard::H:
					break;
				case sf::Keyboard::R:
					break;
				case sf::Keyboard::Escape:
					quit = true;
					break;
				}
			}
		}

		if (!mouseMoveRect.contains(mousePos))
			m_camera.move(normalize(sf::Vector2f(mousePos) - windowCenter)*5.0f);
		//*** controls

		//debugtext
		std::stringstream debugText;
		debugText << mousePos_mapped.x << ":" << mousePos_mapped.y;
		debugText << std::endl << (mouseMoveRect.contains(mousePos) ? "INSIDE" : "OUTSIDE");
		m_debugText.setString(debugText.str());
		//*** debugtext

		//render
		m_window->clear(sf::Color(69, 69, 69));

		m_window->setView(m_camera);

		m_window->draw(m_worldSprite);

		m_window->setView(m_window->getDefaultView());
		m_window->draw(m_debugText);

		m_window->display();
		//*** render
	}
}

void world::setWorldTexture(const char * path)
{
	m_worldTexture.loadFromFile(path);
	m_worldTexture.setRepeated(true);
	m_worldSprite.setTexture(m_worldTexture);
	m_worldSprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
}
