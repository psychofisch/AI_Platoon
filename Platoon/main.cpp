// Platoon.cpp : Defines the entry point for the console application.
//
#include <iostream>

#include <SFML\Graphics.hpp>

#include "world.h"

int main()
{
	//init
	sf::VideoMode vm;
	vm.width = 720;
	vm.height = 720;
	vm.bitsPerPixel = 32;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 2;

	std::cout << "creating window...\n";
	sf::RenderWindow window(vm, "Platoon", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);

	world platoon(sf::Vector2f(2048, 2048), &window);
	platoon.setWorldTexture("checkers_128.png");
	//*** init

	platoon.run();

	//cleanup
    //*** cleanup

	return 0;
}

