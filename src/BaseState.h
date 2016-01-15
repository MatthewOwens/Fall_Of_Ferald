/*
 * The base state is an abstract class, intended to be extended with other classes
 * Contains a SFML packet to pass data between states, since there's not really much
 * point in reinventing the wheel
*/

#ifndef BASESTATE_H
#define BASESTATE_H
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Network/Packet.hpp>
#include "InputManager.h"
class BaseState
{
	public:
	protected:
		BaseState();
		virtual ~BaseState() = 0;
		virtual void update(InputManager& inputManager) = 0;
		virtual void render(sf::RenderWindow& window) = 0;
		virtual void onEnter(sf::Packet& data) = 0;
		virtual sf::Packet onExit() = 0;

		// Packet used to transfer data between states
		sf::Packet bundle;
};
#endif
