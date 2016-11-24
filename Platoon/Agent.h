#pragma once

#include "Kinematics.h"
#include "Steering.h"
#include "gameobj.h"
#include "Path.h"

class Agent : public gameobj
{
	struct steps {
		sf::Vector2f pos[10];
		int cnt;
		float maxTime;
		float curtime;
	};

public:
	enum SteerMode { STEER_FREE = 0, STEER_PATH = 1, STEER_SEPARATION = 2, STEER_COLLISION = 3, STEER_ENEMIES = 4 };

	Agent();
	Agent(const Agent& a);
	~Agent();

	float getMaxSpeed() const;
	float getMaxAcc() const;
	sf::Vector2f getTargetPos() const;
	sf::Vector2f getVelocity() const;
	void getWhiskers(sf::Vector2f* out) const; //out is a 3 vector array (left, center, right)
	std::vector<gameobj>* getObstacles() const;
	std::vector<Agent>* getOtherAgents() const;
	std::vector<Agent>* getEnemies() const;
	Path& getPath();

	void moveTo(sf::Vector2f target);
	void setSteering(SteerMode mode);//accepts SteerMode; int for compatibility
	bool setMaxSpeed(float newMaxSpeed);//accepts Speeds > 0.0f; returns True if value is accepted
	bool setMaxAcc(float newMaxAcc);
	void setSprite(const sf::Texture* tex);
	//void setSprite(std::vector<sf::Texture*>& textures, const char * path);
	void setColor(sf::Color color);
	void setBehaviour(SteerMode mode);
	int addWaypoint(sf::Vector2f p);
	void setObstaclePointer(std::vector<gameobj>* obstacle_ptr);
	void setAgentPointer(std::vector<Agent>* agent_ptr);
	void setEnemyPointer(std::vector<Agent>* enemy_ptr);
	void setRenderWindow(sf::RenderWindow* rndwndw);
	void clearPath();

	void drawDebug(sf::RenderWindow* wndw);
	void drawPath(sf::RenderWindow* wndw);
	virtual void update(float dt);

protected:
	void i_init();

	float m_maxSpeed;
	float m_maxAcc;
	SteerMode m_behaviour;
	sf::Vector2f m_velocity;
	sf::Vector2f m_target;
	std::vector<Steering*> m_steering;
	sf::Sprite m_stepsSprite;
	steps m_steps;
	sf::Color m_color;
	Path m_path;
	std::vector<gameobj>* m_obstacles;
	std::vector<Agent>* m_otherAgents;
	std::vector<Agent>* m_enemies;

	//DEBUG
	sf::RenderWindow* m_wndw;
};

class Formation : public Agent {
public:
	int addAgents(Agent& a);
	void update(float dt);

	std::vector<Agent>& getAgents();
private:
	std::vector<Agent> m_agents;
};
