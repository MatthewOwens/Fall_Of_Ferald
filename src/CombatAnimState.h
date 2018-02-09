#ifndef COMBATANIMSTATE_H
#define COMBATANIMSTATE_H
#include "BaseState.h"
#include "InputManager.h"

class StateManager;

class CombatAnimState : public BaseState
{
public:
    CombatAnimState();
    ~CombatAnimState();
    void update(InputManager* inputManager, StateManager* stateManager);
    void render(sf::RenderWindow* window);
    void onEnter(sf::Packet* data, ImageManager* imageManager);
    void onPause();
    sf::Packet onExit(ImageManager* imageManager);
private:
    sf::Packet data;
    sf::RectangleShape rect;

    std::string terrainTypes[2];
    std::string combatantNames[2];
};
#endif//COMBATANIMSTATE_H
