#include "BaseState.h"

BaseState::BaseState(){}
BaseState::~BaseState(){}
void BaseState::update(InputManager* inputManager, StateManager* stateManager){}
void BaseState::render(sf::RenderWindow* window){}
void BaseState::onEnter(sf::Packet* data){}
void BaseState::onPause(){}
sf::Packet BaseState::onExit(){return bundle;}
