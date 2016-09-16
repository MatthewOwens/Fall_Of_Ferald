#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include "BaseState.h"
#include "InputManager.h"
#include "ImageManager.h"
#include <stack>
class StateManager
{
public:
	enum StateEnum {MENU, OPTIONS, LOADING, GAME, COMBAT, DIALOGUE, PAUSE};

	StateManager(InputManager* inputs, ImageManager* images, sf::RenderWindow* win);
	~StateManager();
	StateManager();
	void pushState(StateEnum stateType, sf::Packet* data = NULL);

	// Pop the current state and push a new state to the stack
	//void switchState(StateEnum stateType, sf::Packet* data = NULL);

	bool update();
	void render();
	void popState();
	bool popTo(StateEnum stateType);
private:

	std::stack<BaseState*> stateStack;
	StateEnum currentState;
	InputManager* inputManager;
	ImageManager* imageManager;
	sf::RenderWindow* window;
};
#endif //STATEMANAGER_H
