#pragma once
#include <SFML\Graphics.hpp>

struct gameobj {
	int textureId;
	sf::Sprite sprite;

	sf::Vector2f getPosition() const {
		return sprite.getPosition();
	}
	void setPosition(sf::Vector2f pos) {
		sprite.setPosition(pos);
	}

	float getRotation() const {
		return sprite.getRotation();
	}
	void setRotation(float rot) {
		sprite.setRotation(rot);
	}
};
