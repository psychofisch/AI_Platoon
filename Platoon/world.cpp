#include "world.h"



world::world(sf::RenderWindow* wndw)
	:m_window(wndw),
	m_size(0, 0)
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
	
	sf::Clock time;
	float dt = 0.0f;

	//init squad
	m_squad.setPosition(sf::Vector2f(500.0f, 50.0f));
	m_squad.moveTo(sf::Vector2f(50.0f, 50.0f));
	m_squad.setMaxSpeed(200.0f);
	m_squad.setMaxAcc(3.0f);
	m_squad.setEnemyPointer(nullptr);

	Agent tester;
	m_textures.push_back(new sf::Texture());
	m_textures[m_textures.size() - 1]->loadFromFile("player.png");
	tester.setSprite(m_textures[m_textures.size() - 1]);
	tester.setColor(sf::Color(41, 255, 249));
	tester.setMaxSpeed(300.0f);
	tester.setMaxAcc(10.0f);
	tester.setObstaclePointer(&m_gameobjects);
	tester.setAgentPointer(&m_squad.getAgents());
	tester.setEnemyPointer(&m_enemies);
	tester.setRenderWindow(m_window);

	tester.setPosition(sf::Vector2f(50.0f, 50.0f));
	tester.moveTo(sf::Vector2f(50.0f, 50.0f));

	m_squad.addAgents(tester);

	tester.setPosition(sf::Vector2f(200.f, 200.f));
	tester.moveTo(sf::Vector2f(200.f, 200.f));
	m_squad.addAgents(tester);
	tester.setPosition(sf::Vector2f(100.f, 200.f));
	m_squad.addAgents(tester);
	tester.setPosition(sf::Vector2f(150.f, 200.f));
	m_squad.addAgents(tester);

	//Enemies
	Agent enemy(tester);
	enemy.setEnemyPointer(nullptr);
	enemy.setAgentPointer(nullptr);
	enemy.setColor(sf::Color(255, 229, 42, 255));
	enemy.setPosition(sf::Vector2f(1040.f, 50.0f ));
	enemy.addWaypoint(sf::Vector2f(1040.f, 50.0f ));
	enemy.addWaypoint(sf::Vector2f(1041.f, 50.0f ));
	enemy.addWaypoint(sf::Vector2f(1042.f, 50.0f ));
	enemy.addWaypoint(sf::Vector2f(1040.f, 500.0f));
	enemy.addWaypoint(sf::Vector2f(1041.f, 500.0f));
	enemy.addWaypoint(sf::Vector2f(1042.f, 500.0f));

	m_enemies.push_back(enemy);

	bool quit = false;
	while (!quit)
	{
		time.restart();

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
				//for(auto&& a : m_squad)
				//	a.moveTo(mousePos_mapped);
				m_squad.moveTo(mousePos_mapped);
				std::cout << mousePos_mapped.x << "," << mousePos_mapped.y << std::endl;
				break;
			}
			else if (eve.type == sf::Event::MouseButtonPressed && eve.mouseButton.button == sf::Mouse::Right)
			{
				//for (auto&& a : m_squad)
				//	a.addWaypoint(mousePos_mapped);
				m_squad.addWaypoint(mousePos_mapped);
				std::cout << mousePos_mapped.x << "," << mousePos_mapped.y << std::endl;
				break;
			}
			else if (eve.type == sf::Event::MouseWheelScrolled)
			{
				//std::cout << "wheel delta > " << eve.mouseWheelScroll.delta << std::endl;
				if (eve.mouseWheelScroll.delta < 0)
				{
					m_camera.zoom(1.1f);
				}
				else if (eve.mouseWheelScroll.delta > 0)
				{
					m_camera.zoom(0.9f);
				}
				break;
			}
			else if (eve.type == sf::Event::KeyPressed)
			{
				switch (eve.key.code)
				{
				case sf::Keyboard::H:
					//std::cout << m_squad[0].getPosition().x << ":" << m_squad[0].getPosition().y << std::endl;
					std::cout << m_squad.getPosition().x << ":" << m_squad.getPosition().y << std::endl;
					break;
				case sf::Keyboard::R:
					m_squad.clearPath();
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
			m_camera.move(normalize(sf::Vector2f(mousePos) - windowCenter)*500.0f*dt);
		}
		//*** controls

		//debugtext
		//std::stringstream debugText;
		//debugText << mousePos_mapped.x << ":" << mousePos_mapped.y;
		//debugText << std::endl << (mouseMoveRect.contains(mousePos) ? "INSIDE" : "OUTSIDE");
		//m_debugText.setString(debugText.str());
		//*** debugtext

		//render
		m_window->clear(sf::Color(69, 69, 69));

		m_window->setView(m_camera);

		m_window->draw(m_worldSprite);

		for (auto&& obstacle : m_gameobjects)
		{
			m_window->draw(obstacle.sprite);
		}

		for (auto&& e : m_enemies)
		{
			e.update(dt);
			m_window->draw(e.sprite);
		}

		m_squad.update(dt);
		m_squad.drawPath(m_window);
		//for (auto&& agent : m_squad)
		for (auto&& agent : m_squad.getAgents())
		{
			//agent.update(dt);
			m_window->draw(agent.sprite);
			agent.drawDebug(m_window);
		}

		m_window->setView(m_window->getDefaultView());
		m_window->draw(m_debugText);

		m_window->display();
		//*** render

		dt = time.getElapsedTime().asSeconds();
	}
}

int world::addTexture(const char * path)
{
	sf::Texture* tmp = new sf::Texture();
	tmp->loadFromFile(path);
	m_textures.push_back(tmp);
	return m_textures.size() - 1;
}

int world::addObstacle(sf::Texture * texture, sf::Vector2f position)
{
	return 0;
}

bool world::loadLevel(const char * path)
{
	if (m_textures.size() > 0)
	{
		for (auto t : m_textures)
		{
			delete t;
		}
	}

	m_size = FileIO::LoadLevel(path, m_gameobjects, m_textures);
	if(m_size.x <= 0.0f || m_size.y <= 0.0f)
		return false;
	
	m_worldSprite.setTexture(*m_textures[0]);
	m_worldSprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));

	for (gameobj g : m_gameobjects)
	{
		m_scene.push_back(&g);
	}

	return true;
}
