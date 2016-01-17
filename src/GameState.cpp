#include "GameState.h"
#include "StateManager.h"
#include <iostream>

GameState::GameState()
{
}

GameState::~GameState()
{
}

void GameState::update(InputManager* inputManager, StateManager* stateManager)
{
    std::vector<sf::Vector3i> locations;
	std::stack<sf::Vector2i> path;

	// Updating the input manager
	//inputManager.update(window);

	// Updating the camera
	if(inputManager->keyHeld("right"))
		camera.move(3,0);
	if(inputManager->keyHeld("left"))
		camera.move(-3,0);
	if(inputManager->keyHeld("down"))
		camera.move(0,3);
	if(inputManager->keyHeld("up"))
		camera.move(0,-3);

	// Updating the UI
	ui.update(inputManager->getMousePosition(), inputManager->getPrevMousePosition(),
			  inputManager->pressedOnce(sf::Mouse::Left), inputManager->pressedOnce(sf::Mouse::Right));

	// Updating the level
	level->update(*inputManager, ui);

	// Checking if the window's been closed
	if(inputManager->pressedOnce("cancel"))
	{
		std::cout << "Closing...!" << std::endl;
		//stateManager->switchState(stateManager->MENU);
		stateManager->switchState(StateManager::MENU);
	}
}

void GameState::render(sf::RenderWindow* window)
{
	if(window != NULL)
	{
		window->clear();
		window->setView(camera);

		if(level != NULL)
			level->draw(*window);

		ui.draw(window, camera);
		window->display();
	}
}

void GameState::onEnter(sf::Packet* data)
{
	if(data != NULL)
	{
		// TODO: parse data
	}
}

sf::Packet GameState::onExit()
{
}

void GameState::onPause()
{
}
