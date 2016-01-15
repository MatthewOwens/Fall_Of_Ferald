#ifndef STATEMANAGER_H
#define STATEMANAGER_H
#include "BaseState.h"
#include <stack>
class StateManager
{
public:
	enum StateEnum {MENU, OPTIONS, LOADING, GAME, COMBAT};

	StateManager();
	~StateManager();
	void pushState(StateEnum stateType);

	// Pop the current state and push a new state to the stack
	void switchState(StateEnum stateType);
private:
	std::stack<BaseState> stateStack;
	StateEnum currentState;
	void popState();
};
#endif //STATEMANAGER_H
