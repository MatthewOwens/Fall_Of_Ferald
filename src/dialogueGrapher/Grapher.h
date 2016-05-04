#ifndef GRAPHER_H
#define GRAPHER_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "InputManager.h"
#include "UserInterface.h"
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
		InputManager inputManager;
		UserInterface ui;
		//ImageManager imageManager;
		sf::RenderWindow window;
		sf::Event event;
};
#endif//GRAPHER_H
