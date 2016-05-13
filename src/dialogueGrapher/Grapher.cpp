#include "Grapher.h"
#include <iostream>
Grapher::Grapher()
{
	window.create(sf::VideoMode(1280, 720), "DialogueGrapher");

	// Loading the font
	font.loadFromFile("assets/fonts/EaseOfUse.ttf");

	nodeView = new NodeView("test", 0, sf::Vector2f(500,50), font);
	ibox = InputBox(sf::Vector2f(window.getSize().x - 300,window.getSize().y - 50), sf::Vector2f(280,25), font);
	inputState = InputState::NONE;

	// Defining the UI colors
	colors["button"] = sf::Color(142, 196, 137);
	colors["buttonHighlight"] = sf::Color(179,80,80);
	colors["background"] = sf::Color(31,31,31);
	colors["graphBG"] = sf::Color(43,43,43);

	moduleName.setString("untitled");
	moduleName.setFont(font);
	moduleName.setCharacterSize(20);
	moduleName.setColor(sf::Color::White);

	sf::Vector2f bgSize(300, window.getSize().y - 20);
	graphBG = sf::RectangleShape(bgSize);
	graphBG.setFillColor(colors["graphBG"]);
	graphBG.setPosition(window.getSize().x - 310,10);
	moduleName.setPosition(graphBG.getPosition());

	// Initilising the buttons
	buttons["m.name"] = new Button(sf::Vector2f(90,20), colors["button"],1);
	buttons["m.name"]->setPosition(sf::Vector2f(window.getSize().x - 310, 100));

	buttons["n.node"] = new Button(sf::Vector2f(80,20), colors["buton"],1);
	buttons["n.node"]->setPosition(sf::Vector2f(window.getSize().x - 310, 200));

	// Setting the button text
	for(auto i : buttons)
		i.second->setText(i.first, font);
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
	sf::Event event;

	// Updating our key input
	inputManager.update(window);

	if(inputManager.pressedOnce("cancel"))
		close = true;

	if(inputManager.pressedOnce(sf::Mouse::Left))
		ibox.checkClicked(inputManager.getMousePosition());

	while(window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::TextEntered:
			{
				if(ibox.isSelected())
				{
					if(event.text.unicode < 128 && event.text.unicode > 31)
					{
						ibox.addCharacter(static_cast<char>(event.text.unicode));
					}
					else if(event.text.unicode == 8) //Backspace
					{
						ibox.removeCharacter();
					}
					else if(event.text.unicode == 13) //Return
					{
						// Choosing what to update
						switch (inputState)
						{
							case NAME:
								moduleName.setString(ibox.getString());
								break;
							case SAVE:
								break;
							case LOAD:
								break;
						}
						inputState = InputState::NONE;
						ibox.setSelected(false);
						ibox.clear();
					}
				}
				break;
			}
		}
	}

	// Checking if any of the buttons have been pressed
	for(auto i : buttons)
	{
		i.second->update(&inputManager);

		if(i.second->isHeld() || i.second->isPressed())
		{
			i.second->setColor(colors["background"]);

			// Flagging the input box as selected if needed
			if(i.first != "n.node" && i.first != "load")
				ibox.setSelected(true);

			// Updating the input state based on what button was pressed
			if(i.first == "m.name")
			{
				inputState = InputState::NAME;
			}
		}
		else i.second->setColor(colors["button"]);

		if(i.second->isMouseOver())
			i.second->setHighlight(colors["buttonHighlight"]);
		else i.second->clearHighlight();
	}
	
}

void Grapher::render()
{
	//window.clear(sf::Color(43,43,43));
	window.clear(sf::Color(31,31,31));
	nodeView->render(window);
	window.draw(graphBG);
	window.draw(moduleName);
	ibox.render(window);

	for(auto i : buttons)
		i.second->draw(&window);
	window.display();
}
