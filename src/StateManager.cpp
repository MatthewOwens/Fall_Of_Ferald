#include "StateManager.h"
#include <iostream>

StateManager::StateManager(InputManager* inputs, ImageManager* images)
{
	// Setting up our pointers
	inputManager = inputs;
	imageManager = images;

	// Creating the game state right off the bat, since it's all we have
	// implemented currently
	pushState(StateEnum::GAME);
}

StateManager::StateManager()
{
	inputManager = NULL;
	imageManager = NULL;
}

StateManager::~StateManager()
{
	// Clearing our stack
	while(!stateStack.empty())
		popState();
}

void StateManager::pushState(StateEnum stateType)
{
	// Pausing the current state
	stateStack.top().onPause();

	// Creating a new state on top of the stack
	switch(stateType)
	{
		default:
			std::cout << "Cannot create state!" << std::endl;
	}
	currentState = stateType;
}


// Removing the current state from the top of the stack
void StateManager::popState()
{
	stateStack.top().onExit();
	stateStack.pop();
}

// Switch states safely, removing the current state and emplacing a new one
// usually called from within the current state
void StateManager::switchState(StateEnum stateType)
{
	stateStack.top().onExit();
	//TODO: Create a new state, call update and render
	stateStack.pop();
	pushState(stateType);
}
