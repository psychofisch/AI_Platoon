#include "Agent.h"


Agent::Agent()
{
	m_behaviour = STEER_FREE;

	m_steering.push_back(new Arrive());
	m_steering.push_back(new Seek());

	sf::Texture* steps_tex = new sf::Texture();
	steps_tex->loadFromFile("steps.png");

	m_stepsSprite.setTexture(*steps_tex);
	m_stepsSprite.setOrigin(steps_tex->getSize().x / 2, steps_tex->getSize().y / 2);

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

void Agent::drawSteps(sf::RenderWindow * wndw)
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
		m_stepsSprite.setColor(sf::Color(77, 255, 54, 255));
		for (int i = 0; i < m_path.size(); ++i)
		{
			m_stepsSprite.setPosition(m_path.getWaypoint(i));
			wndw->draw(m_stepsSprite);
		}
		m_stepsSprite.setPosition(m_path.getNextWaypoint());
		m_stepsSprite.setColor(sf::Color::Red);
		wndw->draw(m_stepsSprite);
	}
	else
	{
		m_stepsSprite.setPosition(m_target);
		m_stepsSprite.setColor(sf::Color::Red);
		wndw->draw(m_stepsSprite);
	}

	//reset
	m_stepsSprite.setColor(sf::Color::White);
}

void Agent::update(float dt)
{
	//movement
	Kinematics latest = m_steering[m_behaviour]->getKinematics(*this);
	
	if (latest.move == true)
	{
		m_velocity += latest.linearAcc;

		if (magnitude(m_velocity) > m_maxSpeed)
			m_velocity = normalize(m_velocity) * m_maxSpeed;

		setPosition(getPosition() + (m_velocity * dt));
		setRotation(getRotation() + (latest.angular * dt));

		//std::cout << magnitude(m_velocity) << std::endl;

		//steps
		m_steps.curtime += dt;
		if (m_steps.curtime >= m_steps.maxTime)
		{
			m_steps.curtime = 0.0f;
			m_steps.pos[m_steps.cnt] = getPosition()/* + sprite.getOrigin()*/;
			++m_steps.cnt;
			if (m_steps.cnt >= 10)
				m_steps.cnt = 0;
		}
	}
}
