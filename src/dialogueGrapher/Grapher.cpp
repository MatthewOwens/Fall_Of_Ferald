#include "Grapher.h"
Grapher::Grapher()
{
	window.create(sf::VideoMode(1280, 720), "Dialogue Grapher");
}

Grapher::~Grapher()
{
}

void Grapher::run()
{
	window.setFramerateLimit(60);

	// main loop
	while(!close)
	{
		update();
		render();
	}
}

void Grapher::update()
{
	// Updating our key input
	inputManager.update(window);

	if(inputManager.pressedOnce("cancel"))
		close = true;
}

void Grapher::render()
{
	window.clear();
	window.display();
}
