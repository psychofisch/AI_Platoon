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
	sf::RenderWindow window(vm, "Platoon", sf::Style::Titlebar | sf::Style::Close | sf::Style::Default, settings);
	window.setVerticalSyncEnabled(false);
	window.setFramerateLimit(60);

	world platoon(&window);
	platoon.loadLevel("level1.x");
	platoon.loadFormationPath("path.x");
	//*** init

	platoon.run();

	return 0;
}

