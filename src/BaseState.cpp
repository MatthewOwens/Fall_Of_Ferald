#include "BaseState.h"

BaseState::BaseState(){}
BaseState::~BaseState(){}
void BaseState::update(InputManager& inputManager){}
void BaseState::render(sf::RenderWindow& window){}
void BaseState::onEnter(sf::Packet& data){}
sf::Packet BaseState::onExit(){return bundle;}
