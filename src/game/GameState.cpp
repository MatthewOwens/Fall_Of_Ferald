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
	inputManager->addBinding("test", sf::Keyboard::T);

	// Updating the camera
	updateCameraRect();

	if (inputManager->keyHeld("right"))
	{
		if (cameraRect.left + cameraRect.width < cameraBounds.x - 3)
			camera.move(3,0);
	}
	if (inputManager->keyHeld("left"))
	{
		if (cameraRect.left >= 3)
			camera.move(-3,0);
	}
	if (inputManager->keyHeld("down"))
	{
		if (cameraRect.top + cameraRect.height < cameraBounds.y - 3)
			camera.move(0,3);
	}
	if (inputManager->keyHeld("up"))
	{
		if (cameraRect.top >= 3)
			camera.move(0,-3);
	}

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

	if (inputManager->pressedOnce("test"))
	{
		sf::Packet bundle;
		bundle << "test";

		stateManager->pushState(StateManager::DIALOGUE, &bundle);
	}

	inputManager->removeBinding("test");
}

void GameState::updateCameraRect()
{
	const sf::Vector2f& viewCenter = camera.getCenter();
	const sf::Vector2f& viewRect = camera.getSize();

	cameraRect.left = viewCenter.x - (viewRect.x / 2);
	cameraRect.top = viewCenter.y - (viewRect.y / 2);
	cameraRect.width = viewRect.x;
	cameraRect.height = viewRect.y;
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

	cameraBounds = sf::Vector2i(level->getMapSizeX() * level->getTileSize(),
								level->getMapSizeY() * level->getTileSize());

	std::cout << "Camera Bounds: (" << cameraBounds.x << "," << cameraBounds.y << ")" << std::endl;

    level->initilizeAI("levels/example/units.txt", "assets/images/unitSprites", *imageManager);

	// Loading the interface data
	//imageManager.loadImage("assets/images/interface/tooltip.png", "tooltip");
	//imageManager.loadImage("assets/images/interface/Dialogue.png", "dialogueBox");
	ui.loadAssets(*imageManager);

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
