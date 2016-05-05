#include "Grapher.h"
Grapher::Grapher()
{
	window.create(sf::VideoMode(1280, 720), "DialogueGrapher");
	nodeView = new NodeView("test", 0, sf::Vector2f(500,50), ui.getFont());
}

Grapher::~Grapher()
{
	delete nodeView;
	nodeView = NULL;
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
	
	ui.update(&inputManager);
}

void Grapher::render()
{
	window.clear(sf::Color(43,43,43));
	//window.clear(sf::Color(31,31,31));
	nodeView->render(window);
	ui.render(window);
	window.display();
}
