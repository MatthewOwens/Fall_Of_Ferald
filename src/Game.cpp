#include "Game.h"
#include <SFML/System/Vector2.hpp>
#include <list>

Game::Game()
{
    window.create(sf::VideoMode(1280, 720), "Fall of Ferald - Refactoring");
}

Game::~Game()
{
    delete testLevel;
}

void Game::run()
{
    testLevel = new Level("levels/example/map.txt", "assets/images/tileSheets/spriteSheet.png", &imageManager);
    testLevel->initilizeAI("levels/example/units.txt", "assets/images/unitSprites", imageManager);

	// Loading the interface data
	//imageManager.loadImage("assets/images/interface/tooltip.png", "tooltip");
	//imageManager.loadImage("assets/images/interface/Dialogue.png", "dialogueBox");
	ui.loadAssets(imageManager);
	ui.addDialogueBox("dialogue/test.txt", 0, 500);

    //float lastTime = 0;
    //sf::Clock clock;
    window.setFramerateLimit(60);

	// Setting up the camera
	camera.reset(sf::FloatRect(0,0,1280,720));

    while (!close)
    {
        update();
        render();
    }
}

void Game::update()
{
    std::vector<sf::Vector3i> locations;
	std::stack<sf::Vector2i> path;

	// Updating the input manager
	inputManager.update(window);

	// Updating the camera
	if(inputManager.keyHeld("right"))
		camera.move(3,0);
	if(inputManager.keyHeld("left"))
		camera.move(-3,0);
	if(inputManager.keyHeld("down"))
		camera.move(0,3);
	if(inputManager.keyHeld("up"))
		camera.move(0,-3);

	// Updating the UI
	ui.update(inputManager.getMousePosition(), inputManager.getPrevMousePosition(),
			  inputManager.pressedOnce(sf::Mouse::Left), inputManager.pressedOnce(sf::Mouse::Right));

	// Updating the level
	testLevel->update(inputManager, ui);

	// Checking if the window's been closed
	if(inputManager.pressedOnce("cancel"))
	{
		std::cout << "Closing...!" << std::endl;
		close = true;
	}
}

void Game::render()
{
	// Clearing the window to prevent ghosting from the previous frame
    window.clear();

	// Binding the window view to the camera
	window.setView(camera);
    testLevel->draw(window);

	// Resetting the window view for our UI
//	window.setView(window.getDefaultView());
	ui.draw(&window, camera);

    window.display();
}
