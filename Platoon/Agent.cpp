#include "Agent.h"



Agent::Agent()
	:m_steering(nullptr)
{
}

Agent::Agent(int mode)
{
	if (!setSteering(mode))
		m_steering = nullptr;
}


Agent::~Agent()
{
	delete m_steering;
}

float Agent::getMaxSpeed()
{
	return m_maxSpeed;
}

void Agent::moveTo(sf::Vector2f target)
{
	

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
}
