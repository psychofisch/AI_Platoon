#pragma once

#include "Kinematics.h"
#include "Steering.h"
#include "gameobj.h"

class Agent : public gameobj
{
	struct steps {
		sf::Vector2f pos[10];
		int cnt;
		float maxTime;
		float curtime;
	};

public:
	enum SteerMode { STEER_NONE, STEER_ARRIVE };

	Agent();
	Agent(int mode);
	~Agent();

	float getMaxSpeed() const;
	float getMaxAcc() const;
	sf::Vector2f getTargetPos() const;
	sf::Vector2f getVelocity() const;
	
	void moveTo(sf::Vector2f target);
	bool setSteering(int mode);//accepts SteerMode; int for compatibility
	bool setMaxSpeed(float newMaxSpeed);//accepts Speeds > 0.0f; returns True if value is accepted
	bool setMaxAcc(float newMaxAcc);
	void setSprite(std::vector<sf::Texture*>& textures, const char * path);
	void setColor(sf::Color color);

	void drawSteps(sf::RenderWindow* wndw);
	void update(float dt);

private:
	float m_maxSpeed;
	float m_maxAcc;
	sf::Vector2f m_velocity;
	sf::Vector2f m_target;
	Steering* m_steering;
	sf::Sprite m_stepsSprite;
	steps m_steps;
	sf::Color m_color;
	std::vector<sf::Vector2f> m_path;
};
