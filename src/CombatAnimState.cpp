#include "CombatAnimState.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "StateManager.h"
#include "termcolor.hpp"


CombatAnimState::CombatAnimState()
{
    // Ensuring that the strings are initilised to something
    for(int i = 0; i < 2; ++i)
    {
        terrainTypes[i] = "NO TERRAIN";
        combatantNames[i] = "NO COMBATANT";
    }

    rect.setSize(sf::Vector2f(1280,720));
    rect.setPosition(0,0);
    rect.setFillColor(sf::Color(43,43,43,150));
}

CombatAnimState::~CombatAnimState()
{
}

void CombatAnimState::update(InputManager* inputManager, StateManager* stateManager)
{
    if(inputManager->pressedOnce("cancel"))
        stateManager->popState();
}

void CombatAnimState::render(sf::RenderWindow* window)
{
    window->draw(rect);
}

void CombatAnimState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
    renderPrevious = true;

    std::cout << termcolor::green << "Combat state entered" << termcolor::reset
    << std::endl;

    // Unpacking data from the packet
    if(data != NULL)
    {
        *data >> combatantNames[0];
        *data >> terrainTypes[0];
        *data >> combatantNames[1];
        *data >> terrainTypes[1];
    }

    std::cout << termcolor::yellow << combatantNames[0] << " is on " <<
    terrainTypes[0] << " and attacked " << combatantNames[1] << " on " <<
    terrainTypes[1] << termcolor::reset << std::endl;
}

void CombatAnimState::onPause()
{
}

sf::Packet CombatAnimState::onExit(ImageManager* imageManager)
{
    return data;
}
