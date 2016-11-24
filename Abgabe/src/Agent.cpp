#include "Agent.h"


Agent::Agent()
{
	i_init();
}

Agent::Agent(const Agent & a)
	:m_maxAcc(a.m_maxAcc),
	m_maxSpeed(a.m_maxSpeed),
	m_behaviour(a.m_behaviour),
	m_velocity(a.m_velocity),
	m_target(a.m_target),
	m_steps(a.m_steps),
	m_path(a.m_path),
	m_obstacles(a.m_obstacles),
	m_otherAgents(a.m_otherAgents),
	m_enemies(a.m_enemies),
	m_wndw(a.m_wndw)
{
	i_init();

	sf::Texture* steps_tex = new sf::Texture(*(a.m_stepsSprite.getTexture()));

	m_stepsSprite.setTexture(*steps_tex);
	m_stepsSprite.setOrigin(steps_tex->getSize().x / 2, steps_tex->getSize().y / 2);
	m_stepsSprite.scale(0.5f, 0.5f);

	setSprite(a.sprite.getTexture());

	setColor(a.m_color);
	setPosition(a.getPosition());
	moveTo(a.getPosition());
}

Agent::~Agent()
{
	for (Steering* s : m_steering)
	{
		delete s;
	}
}

void Agent::i_init()
{
	if(m_behaviour < 0 || m_behaviour > 4)
	m_behaviour = STEER_FREE;

	m_steering.resize(5);
	m_steering[STEER_FREE] = new Arrive();
	m_steering[STEER_PATH] = new Seek();
	m_steering[STEER_SEPARATION] = new Separation();
	m_steering[STEER_COLLISION] = new ObstacleAvoid();
	m_steering[STEER_ENEMIES] = new CollisionAvoid();

	sf::Texture* player_tex = new sf::Texture();
	player_tex->loadFromFile("player.png");

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

std::vector<Agent>* Agent::getOtherAgents() const
{
	return m_otherAgents;
}

std::vector<Agent>* Agent::getEnemies() const
{
	return m_enemies;
}

Path& Agent::getPath()
{
	return m_path;
}

void Agent::moveTo(sf::Vector2f target)
{
	m_target = target;
}

void Agent::setSteering(SteerMode mode)
{
	m_behaviour = mode;
}

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

void Agent::setSprite(const sf::Texture * tex)
{
	sprite.setTexture(*tex);
	sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);
}

void Agent::setColor(sf::Color color)
{
	m_color = color;
	sprite.setColor(m_color);
}

void Agent::setBehaviour(SteerMode mode)
{
	m_behaviour = mode;
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

void Agent::setAgentPointer(std::vector<Agent>* agent_ptr)
{
	m_otherAgents = agent_ptr;
}

void Agent::setEnemyPointer(std::vector<Agent>* enemy_ptr)
{
	m_enemies = enemy_ptr;
}

void Agent::setRenderWindow(sf::RenderWindow * rndwndw)
{
	m_wndw = rndwndw;
}

void Agent::clearPath()
{
	m_path = Path();
	m_behaviour = STEER_FREE;
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
	drawPath(wndw);

	m_stepsSprite.setPosition(m_target);
	m_stepsSprite.setColor(sf::Color::Red);
	wndw->draw(m_stepsSprite);

	//draw whiskers
	sf::Vector2f w[3];
	getWhiskers(w);
	for (int i = 0; i < 3; ++i)
	{
		sf::Vector2f line(w[i]);
		sf::RectangleShape lineShape(sf::Vector2f(magnitude(line), 2));
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

void Agent::drawPath(sf::RenderWindow * wndw)
{
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
}

void Agent::update(float dt)
{
	//movement
	Kinematics doIt = m_steering[STEER_COLLISION]->getKinematics(*this);

	if(doIt.move != true)
		doIt = m_steering[STEER_SEPARATION]->getKinematics(*this);

	if (doIt.move != true)
		doIt = m_steering[STEER_ENEMIES]->getKinematics(*this);

	if (doIt.move != true)
		doIt = m_steering[m_behaviour]->getKinematics(*this);

	if (m_behaviour == STEER_PATH)
		m_target = m_path.getNextWaypoint();
	
	if (doIt.move == true)
	{
		m_velocity += doIt.linearAcc;

		if (magnitude(m_velocity) > m_maxSpeed)
			m_velocity = normalize(m_velocity) * m_maxSpeed;

		setPosition(getPosition() + (m_velocity * dt));
		setRotation(getRotation() + (doIt.angular * dt));

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

int Formation::addAgents(Agent& a)
{
	a.setSteering(Agent::STEER_FREE);
	m_agents.push_back(a);

	return m_agents.size();
}

void Formation::update(float dt)
{
	Kinematics target = m_steering[m_behaviour]->getKinematics(*this);

	if (target.move == true)
	{
		if (m_behaviour == STEER_PATH)
			m_target = m_path.getNextWaypoint();

		m_velocity += target.linearAcc;

		sf::Vector2f brake;
		for (auto&& a : m_agents)
		{
			brake += (getPosition() - a.getPosition());
		}
		brake /= 4.0f;

		float brake_f = 300.f;
		if (magnitude(brake) > brake_f)
		{
			float brake_val = std::min((magnitude(brake) - brake_f) / brake_f, 1.0f);
			m_velocity = (m_velocity * (1.f - brake_val));
		}

		if (magnitude(m_velocity) > m_maxSpeed)
			m_velocity = normalize(m_velocity) * m_maxSpeed;

		setPosition(getPosition() + (m_velocity * dt));
		setRotation(getRotation() + (target.angular * dt));

		for (int i = 0; i < m_agents.size(); ++i)
		{
			sf::Vector2f wedge;
			switch (i)
			{
			case 1: wedge = normalize(m_velocity) * 100.f + sf::Vector2f(normalize(m_velocity).y, -normalize(m_velocity).x) * 100.0f;
				break;
			case 2: wedge = normalize(m_velocity) * 100.f + sf::Vector2f(normalize(m_velocity).y, -normalize(m_velocity).x) * -100.0f;
				break;
			case 3: wedge = normalize(m_velocity) * 200.f + sf::Vector2f(normalize(m_velocity).y, -normalize(m_velocity).x) * 200.0f;
				break;
			}

			m_agents[i].moveTo(getPosition() - wedge);
			m_agents[i].update(dt);
		}
	}
}

std::vector<Agent>& Formation::getAgents()
{
	return m_agents;
}
