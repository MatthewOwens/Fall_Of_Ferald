#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

#include "BaseState.h"
#include "ImageManager.h"
#include "InputManager.h"
#include "GameUserInterface.h"
#include "Pathfinder.h"
#include "Level.h"
#include "AI.h"

class StateManager;

class GameState : public BaseState
{
public:
	GameState();
	~GameState();
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	sf::Packet onExit(ImageManager* imageManager);

private:
	GameUserInterface ui;
	Level* level;

	sf::View camera;
	sf::Vector2i cameraBounds;
	sf::FloatRect cameraRect;

	void updateCameraRect();

	bool closed = false;
	bool turnPassed = false;
	const std::string tilesheetPath = "assets/images/tileSheets/";
};
#endif //GAMESTATE_H
