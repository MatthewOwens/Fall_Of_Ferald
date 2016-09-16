#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include <SFML/Graphics/RectangleShape.hpp>
#include "BaseState.h"
#include "Button.h"
#include <string>
#include <map>

class StateManager;

class PauseState : public BaseState
{
public:
	PauseState();
	~PauseState();
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	sf::Packet onExit(ImageManager* imageManager);
private:
	sf::Vector2u windowBounds;
	sf::RectangleShape background;
	std::map<std::string, Button*> buttons;
	sf::Font buttonFont;
};
#endif//PAUSESTATE_H
