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

	m_camera = sf::View(sf::FloatRect(0, 0, wndw->getSize().x, wndw->getSize().y));
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
			else if (eve.type == sf::Event::MouseButtonPressed && eve.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << mousePos_mapped.x << "," << mousePos_mapped.y << std::endl;
				break;
			}
			else if (eve.type == sf::Event::MouseWheelScrolled)
			{
				//std::cout << "wheel delta > " << eve.mouseWheelScroll.delta << std::endl;
				if (eve.mouseWheelScroll.delta < 0)
				{
					//zoomFactor += 0.1f;
					m_camera.zoom(1.05f);
				}
				else if (eve.mouseWheelScroll.delta > 0)
				{
					//zoomFactor -= 0.1f;
					m_camera.zoom(0.95f);
				}
				break;
			}
			else if (eve.type == sf::Event::KeyPressed)
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
			else if (eve.type == sf::Event::Resized)
			{
				m_camera = sf::View(sf::FloatRect(0, 0, eve.size.width, eve.size.height));
				//m_camera.setViewport(sf::FloatRect(0, 0, eve.size.width, eve.size.height));
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && !mouseMoveRect.contains(mousePos))
		{
			m_camera.move(normalize(sf::Vector2f(mousePos) - windowCenter)*5.0f);
		}
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

		for (auto obstacle : m_obstacles)
		{
			m_window->draw(obstacle.sprite);
		}

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

int world::addObstacle(sf::Texture * texture, sf::Vector2f position)
{
	return 0;
}

bool world::loadObstacles(const char * path)
{
	return FileIO::LoadObstacles(path, m_obstacles, m_textures);
}
