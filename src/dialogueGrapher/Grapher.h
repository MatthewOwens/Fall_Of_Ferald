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
#include "FileManager.h"

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
		void update();
		void render();

		bool close = false;
		sf::View graphView;
		InputManager inputManager;
		FileManager fileManager;
		InputBox ibox;	// The InputBox used to get input for this class
		sf::Text moduleName;	// Name of the module being edited, defaults to untitled
		sf::Font font;
		sf::RectangleShape graphBG;	// Box to the left of the screen, just there
									// so we have a place to put the UI
		std::map<std::string, sf::Color> colors;	// UI Colors
		std::map<std::string, Button*> buttons;		// UI Buttons
		std::list<NodeView*> nodeViews;		// Representations of the nodes in this module
		sf::RenderWindow window;
		sf::Event event;
		int nodeCount;
		float scale;
		bool movingView;
		bool showNodeNames;

		NodeView* selectedNode;
		InputBox* selectedInputBox;

		NodeView* connectingNodes[2];	// Array of pointers for storing the nodes
										// are to be connected with a new connection
};
#endif//GRAPHER_H
