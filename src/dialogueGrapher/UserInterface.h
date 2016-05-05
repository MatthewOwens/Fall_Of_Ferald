#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <map>
#include "Button.h"
#include "InputManager.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
class UserInterface
{
	public:
		UserInterface();
		~UserInterface();
		void update(InputManager* inputManager);
		void render(sf::RenderWindow& window);
		const sf::Font& getFont() { return font; }

	private:
		std::map<std::string, sf::Color> colors;
		std::map<std::string, Button*> buttons;
		sf::Font font;
};
#endif//USERINTERFACE_H
