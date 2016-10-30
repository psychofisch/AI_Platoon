#pragma once
#include <SFML\Graphics.hpp>

struct gameobj {
	int textureId;
	sf::Sprite sprite;
	sf::Vector2f getPosition() {
		return sprite.getPosition();
	}
	void setPosition(sf::Vector2f& pos) {
		sprite.setPosition(pos);
	}

	sf::Vector2f getRotation() {
		sprite.getRotation();
	}
	void setRotation(float& rot) {
		return sprite.setRotation(rot);
	}
};
