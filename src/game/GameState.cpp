#include "GameState.h"
#include "StateManager.h"
#include <iostream>

GameState::GameState() : BaseState()
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
		//stateManager->switchState(StateManager::MENU);
		stateManager->popState();
	}
}

void GameState::render(sf::RenderWindow* window)
{
	if(window != NULL)
	{
		window->setView(camera);

		if(level != NULL)
			level->draw(*window);

		ui.draw(window, camera);
	}
}

void GameState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	std::cout << "Game onEnter" << std::endl;
	if(data != NULL)
	{
		// TODO: parse data
	}

    level = new Level("levels/example/map.txt", "assets/images/tileSheets/spriteSheet.png", imageManager);
    level->initilizeAI("levels/example/units.txt", "assets/images/unitSprites", *imageManager);

	// Loading the interface data
	//imageManager.loadImage("assets/images/interface/tooltip.png", "tooltip");
	//imageManager.loadImage("assets/images/interface/Dialogue.png", "dialogueBox");
	ui.loadAssets(*imageManager);
	ui.addDialogueBox("dialogue/test.txt", 0, 500);

	// Setting up the camera
	camera.reset(sf::FloatRect(0,0,1280,720));
}

sf::Packet GameState::onExit(ImageManager* imageManager)
{
	if(level != NULL)
	{
		delete level;
		level = NULL;
	} else std::cout << "GameState level is null in onExit!" << std::endl;

	return bundle;
}

void GameState::onPause()
{
}
