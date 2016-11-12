#include "Agent.h"



Agent::Agent()
	:Agent(STEER_NONE)
{
}

Agent::Agent(int mode)
{
	if (!setSteering(mode))
		m_steering = nullptr;

	sf::Texture* steps_tex = new sf::Texture();
	steps_tex->loadFromFile("steps.png");

	m_stepsSprite.setTexture(*steps_tex);
	m_stepsSprite.setOrigin(steps_tex->getSize().x / 2, steps_tex->getSize().y / 2);

	m_steps.cnt = 0;
	m_steps.maxTime = 1.0f;
	m_steps.curtime = 0.0f;
}


Agent::~Agent()
{
	delete m_steering;
}

float Agent::getMaxSpeed()
{
	return m_maxSpeed;
}

sf::Vector2f Agent::getTargetPos()
{
	return m_target;
}

void Agent::moveTo(sf::Vector2f target)
{
	m_target = target;
}

bool Agent::setSteering(int mode)
{
	if (mode == STEER_ARRIVE)
	{
		m_steering = new Arrive();
		return true;
	}
	return false;
}

bool Agent::setMaxSpeed(float newMaxSpeed)
{
	if (newMaxSpeed <= 0.0f)
		return false;

	m_maxSpeed = newMaxSpeed;
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

void Agent::drawSteps(sf::RenderWindow * wndw)
{
	for (int i = 0; i < 10; ++i)
	{
		m_stepsSprite.setPosition(m_steps.pos[i]);
		wndw->draw(m_stepsSprite);
	}

	m_stepsSprite.setPosition(m_target);
	m_stepsSprite.setColor(sf::Color::Red);
	wndw->draw(m_stepsSprite);
	m_stepsSprite.setColor(sf::Color::White);
}

void Agent::update(float dt)
{
	//movement
	Kinematics latest = m_steering->getKinematics(*this);
	if (latest.enabled == false)
		return;

	setPosition(getPosition() + latest.velocity * dt);
	setRotation(getRotation() + (latest.rotation * dt));

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
