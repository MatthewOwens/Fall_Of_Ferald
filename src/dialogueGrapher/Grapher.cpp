#include "Grapher.h"
#include <iostream>
Grapher::Grapher()
{
	window.create(sf::VideoMode(1280, 720), "DialogueGrapher");

	// Loading the font
	font.loadFromFile("assets/fonts/EaseOfUse.ttf");

	nodeCount = 0;
	selectedNode = NULL;

	//nodeViews.push_back(new NodeView("test", nodeViews.count(), sf::Vector2f(500,50), font));
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
	/*delete nodeView;
	nodeView = NULL;*/
	for(auto i : nodeViews)
	{
		delete i;
		i = NULL;
	}
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
			
			case sf::Event::MouseButtonReleased:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					selectedNode = NULL;
				}
				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "LMB Pressed!" << std::endl;
					// Selecting a node
					for(auto i : nodeViews)
					{
						if(i->getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
						{
							selectedNode = i;
						}
					}

					// Checking if any of the buttons have been clicked
					for(auto i : buttons)
					{
						i.second->update(&inputManager);

						if(i.second->isPressed())
						{
							i.second->setColor(colors["background"]);

							// Flagging the input box as selected if needed
							if(i.first != "n.node")
								ibox.setSelected(true);

							// Updating the input state based on what button was pressed
							if(i.first == "m.name")
								inputState = InputState::NAME;
							else if(i.first == "save")
								inputState = InputState::SAVE;
							else if(i.first == "load")
								inputState = InputState::LOAD;
							else
							{
								nodeViews.push_back(new NodeView(moduleName.getString(),
									nodeCount, sf::Vector2f(500,50), font));

								// Tracking the total nodes created to prevent repeated IDs
								nodeCount++;
							}

						}
						else i.second->setColor(colors["button"]);

						if(i.second->isMouseOver())
							i.second->setHighlight(colors["buttonHighlight"]);
						else i.second->clearHighlight();
					}
				}
				else if(event.mouseButton.button == sf::Mouse::Right)
				{
					// Removing node if needed
					if(inputManager.pressedOnce(sf::Mouse::Right))
					{
						for(auto i = nodeViews.begin(); i != nodeViews.end(); )
						{
							if((*i)->removeRequired(inputManager.getMousePosition()))
							{
								delete *i;
								*i = NULL;

								i = nodeViews.erase(i);
							}
							else ++i;
						}
					}
				}
			}
			break;
		}
	}

	// Moving nodes
	if(selectedNode)
	{
		selectedNode->move(inputManager.getMousePosition() - inputManager.getPrevMousePosition());
	}
}

void Grapher::render()
{
	//window.clear(sf::Color(43,43,43));
	window.clear(sf::Color(31,31,31));
	//nodeView->render(window);

	for(auto i : nodeViews)
		i->render(window);

	window.draw(graphBG);
	window.draw(moduleName);
	ibox.render(window);

	for(auto i : buttons)
		i.second->draw(&window);
	window.display();
}
