#ifndef GRAPHER_H
#define GRAPHER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <list>
#include <map>
#include "NodeView.h"
#include "InputBox.h"

// If compiling with cmake & make
#ifndef _WIN32
#include "InputManager.h"
#include "Button.h"
#else
#include "../game/InputManager.h"
#include "../game/Button.h"
#endif
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
		//NodeView* nodeView = NULL;
		std::list<NodeView*> nodeViews;
		sf::RenderWindow window;
		sf::Event event;
		int nodeCount;
		float scale;
		bool movingView;
		NodeView* selectedNode;
};
#endif//GRAPHER_H
