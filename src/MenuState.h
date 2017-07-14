#ifndef MENUSTATE_H
#define MENUSTATE_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>

#include "BaseState.h"
#include "ImageManager.h"
#include "InputManager.h"
#include "Button.h"
#include "Juicer.h"

class StateManager;

class MenuState : public BaseState
{
public:
	MenuState();
	~MenuState();
	Juicer juicer;
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	sf::Packet onExit(ImageManager* imageManager);

private:
	sf::Sprite* menubgSprite;
	sf::Font buttonFont;
	Button* button;
	Button* exitButton;
};
#endif //MENUSTATE_H
