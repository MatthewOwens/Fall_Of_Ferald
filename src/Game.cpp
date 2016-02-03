#include "Game.h"
#include <SFML/System/Vector2.hpp>
#include <SFML/OpenGL.hpp>
#include <list>

Game::Game()
{
	// Initilising the OpenGL context
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 2;

    window.create(sf::VideoMode(1280, 720), "Fall of Ferald", sf::Style::Default, settings);
	stateManager = NULL;

	settings = window.getSettings();
	std::cout << "OpenGL context created!" << std::endl;
	std::cout << "\tDepth Bits:\t" << settings.depthBits << std::endl;
	std::cout << "\tStencil Bits:\t" << settings.stencilBits << std::endl;
	std::cout << "\tAA Level:\t" << settings.antialiasingLevel << std::endl;
	std::cout << "\tVersion:\t" << settings.majorVersion << "." << settings.minorVersion << std::endl;
	std::cout << std::endl;
}

Game::~Game()
{
}

void Game::run()
{
    window.setFramerateLimit(60);
	stateManager = new StateManager(&inputManager, &imageManager, &window);

	// Main loop
	while(!close)
	{
		update();
		render();
	}
}

void Game::update()
{
	close = stateManager->update();
}

void Game::render()
{
	if(!close)
		stateManager->render();
}
