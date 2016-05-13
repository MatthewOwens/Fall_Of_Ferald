#ifndef GRAPHER_H
#define GRAPHER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <map>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "NodeView.h"
#include "InputManager.h"
#include "InputBox.h"
#include "Button.h"
class Grapher
{
	public:
		Grapher();
		~Grapher();
		void run();
	
	private:
		enum InputState {NONE, NAME, SAVE, LOAD};
		void update();
		void render();

		bool close = false;
		InputManager inputManager;
		InputBox ibox;
		InputState inputState;
		sf::Text moduleName;
		sf::Font font;
		sf::RectangleShape graphBG;
		std::map<std::string, sf::Color> colors;
		std::map<std::string, Button*> buttons;
		NodeView* nodeView = NULL;
		sf::RenderWindow window;
		sf::Event event;
};
#endif//GRAPHER_H
