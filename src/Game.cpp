#include "Game.h"
#include <SFML/System/Vector2.hpp>
#include <list>

Game::Game()
{
    window.create(sf::VideoMode(1280, 720), "Fall of Ferald");
	stateManager = NULL;
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
	stateManager->render();
}
