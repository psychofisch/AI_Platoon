#include "Agent.h"


Agent::Agent()
{
	m_behaviour = STEER_FREE;

	m_steering.push_back(new Arrive());
	m_steering.push_back(new Seek());
	m_steering.push_back(new ObstacleAvoid());

	sf::Texture* steps_tex = new sf::Texture();
	steps_tex->loadFromFile("steps.png");

	m_stepsSprite.setTexture(*steps_tex);
	m_stepsSprite.setOrigin(steps_tex->getSize().x / 2, steps_tex->getSize().y / 2);
	m_stepsSprite.scale(0.5f, 0.5f);

	m_steps.cnt = 0;
	m_steps.maxTime = 1.0f;
	m_steps.curtime = 0.0f;

	m_color = sf::Color(255, 255, 255);
}


Agent::~Agent()
{
	for (Steering* s : m_steering)
	{
		delete s;
	}
}

float Agent::getMaxSpeed() const
{
	return m_maxSpeed;
}

float Agent::getMaxAcc() const
{
	return m_maxAcc;
}

sf::Vector2f Agent::getTargetPos() const
{
	return m_target;
}

sf::Vector2f Agent::getVelocity() const
{
	return m_velocity;
}

void Agent::getWhiskers(sf::Vector2f * out) const
{
	float angle = 20.f;
	//center ray
	if (magnitude(m_velocity) < sprite.getOrigin().x)
		out[1] = normalize(m_velocity) * sprite.getOrigin().x;
	else
		out[1] = m_velocity;

	//whiskers
	out[0] = rotateD(out[1], angle) * 0.5f;
	out[2] = rotateD(out[1], -angle) * 0.5f;
}

std::vector<gameobj>* Agent::getObstacles() const
{
	return m_obstacles;
}

Path& Agent::getPath()
{
	return m_path;
}

void Agent::moveTo(sf::Vector2f target)
{
	m_target = target;
}

/*bool Agent::setSteering(int mode)
{
	if (mode == STEER_ARRIVE)
	{
		m_steering = new Arrive();
		return true;
	}
	return false;
}*/

bool Agent::setMaxSpeed(float newMaxSpeed)
{
	if (newMaxSpeed <= 0.0f)
		return false;

	m_maxSpeed = newMaxSpeed;
	return true;
}

bool Agent::setMaxAcc(float newMaxAcc)
{
	if (newMaxAcc <= 0.0f)
		return false;

	m_maxAcc = newMaxAcc;
	return true;
}

void Agent::setSprite(std::vector<sf::Texture*>& textures, const char * path)
{
	sf::Texture* tmp_tex = new sf::Texture;
	tmp_tex->loadFromFile(path);
	textures.push_back(tmp_tex);

	sprite.setTexture(*textures[textures.size() - 1]);
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
}

void Agent::setColor(sf::Color color)
{
	m_color = color;
	sprite.setColor(m_color);
}

int Agent::addWaypoint(sf::Vector2f p)
{
	m_behaviour = STEER_PATH;
	return m_path.addWaypoint(p);
}

void Agent::setObstaclePointer(std::vector<gameobj>* obstacle_ptr)
{
	m_obstacles = obstacle_ptr;
}

void Agent::setRenderWindow(sf::RenderWindow * rndwndw)
{
	m_wndw = rndwndw;
}

void Agent::drawDebug(sf::RenderWindow * wndw)
{
	//draw steps
	m_stepsSprite.setColor(m_color);
	for (int i = 0; i < 10; ++i)
	{
		m_stepsSprite.setPosition(m_steps.pos[i]);
		wndw->draw(m_stepsSprite);
	}

	//draw path&target
	if (m_behaviour == STEER_PATH)
	{
		for (int i = 1; i <= m_path.size(); ++i)
		{
			//lines
			sf::Vector2f line(m_path.getWaypoint(i - 1) - m_path.getWaypoint(i));
			sf::RectangleShape lineShape(sf::Vector2f(magnitude(line), 5));
			lineShape.setFillColor(sf::Color::Black);
			lineShape.rotate(angleD(line));
			lineShape.setPosition(m_path.getWaypoint(i));
			wndw->draw(lineShape);
		}
	}

	m_stepsSprite.setPosition(m_target);
	m_stepsSprite.setColor(sf::Color::Red);
	wndw->draw(m_stepsSprite);

	//draw whiskers
	sf::Vector2f w[3];
	getWhiskers(w);
	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2f line(w[i]);
		sf::RectangleShape lineShape(sf::Vector2f(magnitude(line), 5));
		lineShape.setFillColor(sf::Color::Magenta);
		lineShape.setRotation(angleD(line));
		lineShape.setPosition(getPosition());
		wndw->draw(lineShape);
	}

	//debug rectangle corners
	m_stepsSprite.setColor(sf::Color::Red);
	for (gameobj go : *m_obstacles)
	{
		sf::Vector2f rC[4];
		getRealCorners(go.sprite, rC);
		for (int i = 0; i < 4; ++i)
		{
			m_stepsSprite.setPosition(rC[i]);
			wndw->draw(m_stepsSprite);
		}
	}

	//reset
	m_stepsSprite.setColor(sf::Color::White);
}

void Agent::update(float dt)
{
	//movement
	Kinematics latest = m_steering[m_behaviour]->getKinematics(*this);
	Kinematics collision = m_steering[2]->getKinematics(*this);

	Kinematics doIt = latest;
	if (collision.move == true)
		doIt = collision;

	if (m_behaviour == STEER_PATH)
		m_target = m_path.getNextWaypoint();
	
	if (doIt.move == true)
	{
		m_velocity += doIt.linearAcc;

		if (magnitude(m_velocity) > m_maxSpeed)
			m_velocity = normalize(m_velocity) * m_maxSpeed;

		setPosition(getPosition() + (m_velocity * dt));
		setRotation(getRotation() + (doIt.angular * dt));

		//std::cout << magnitude(m_velocity) << std::endl;

		//steps
		m_steps.curtime += dt;
		if (m_steps.curtime >= m_steps.maxTime)
		{
			m_steps.curtime = 0.0f;
			m_steps.pos[m_steps.cnt] = getPosition();
			++m_steps.cnt;
			if (m_steps.cnt >= 10)
				m_steps.cnt = 0;
		}
	}
}
