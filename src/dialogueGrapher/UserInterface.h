#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <map>
#include "Button.h"
#include "InputManager.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
class UserInterface
{
	public:
		UserInterface();
		void initBG(const sf::Vector2u& windowSize);
		~UserInterface();
		void update(InputManager* inputManager);
		void render(sf::RenderWindow& window);
		const sf::Font& getFont() { return font; }

	private:
		std::map<std::string, sf::Color> colors;
		std::map<std::string, Button*> buttons;
		sf::RectangleShape graphBG;
		sf::Font font;
};
#endif//USERINTERFACE_H
