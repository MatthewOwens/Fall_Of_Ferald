#include "Grapher.h"
#include <iostream>
#include <algorithm>
Grapher::Grapher()
{
	window.create(sf::VideoMode(1280, 720), "DialogueGrapher");
	//fileManager.loadDialogue("testModule.json");

	// Loading the font
	font.loadFromFile("assets/fonts/EaseOfUse.ttf");

	nodeCount = 0;
	scaleFactor = 1.f;
	scale = 1.f;
	selectedNode = NULL;
	selectedInputBox = NULL;
	movingView = false;
	showNodeNames = true;

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

	ibox = InputBox(sf::Vector2f(window.getSize().x - 290,window.getSize().y - 50), sf::Vector2f(280,25), font);
	ibox.setActive(false);

	// Defining the UI colors
	colors["button"] = sf::Color(142, 196, 137);
	colors["buttonHighlight"] = sf::Color(179,80,80);
	colors["background"] = sf::Color(31,31,31);
	colors["graphBG"] = sf::Color(43,43,43);

	moduleName.setString("untitled");
	moduleName.setFont(font);
	moduleName.setCharacterSize(20);
	moduleName.setColor(sf::Color::White);

	sf::Vector2f bgSize(300, window.getSize().y);
	graphBG = sf::RectangleShape(bgSize);
	graphBG.setFillColor(colors["graphBG"]);
	graphBG.setPosition(window.getSize().x - 300,0);
	moduleName.setPosition(graphBG.getPosition());

	// Setting the view
	graphView.reset(sf::FloatRect(0,0,window.getSize().x, window.getSize().y));

	// Initilising the buttons
	buttons["m.name"] = new Button(sf::Vector2f(90,20), colors["button"],1);
	buttons["m.name"]->setPosition(sf::Vector2f(window.getSize().x - 290, 100));

	buttons["n.node"] = new Button(sf::Vector2f(80,20), colors["button"],1);
	buttons["n.node"]->setPosition(sf::Vector2f(window.getSize().x - 290, 200));

	buttons["exit"] = new Button(sf::Vector2f(80, 20), colors["button"], 1);
	buttons["exit"]->setPosition(sf::Vector2f(window.getSize().x - 290, 300));

	buttons["n.names"] = new Button(sf::Vector2f(95, 20), colors["button"], 1);
	buttons["n.names"]->setPosition(sf::Vector2f(window.getSize().x - 150, 200));

	buttons["load"] = new Button(sf::Vector2f(80, 20), colors["button"], 1);
	buttons["load"]->setPosition(sf::Vector2f(window.getSize().x - 290, 400));

	// Setting the button text
	for(auto i : buttons)
		i.second->setText(i.first, font);
}

Grapher::~Grapher()
{
	for(auto i : nodeViews)
	{
		delete i;
		i = NULL;
	}
}

void Grapher::run()
{
	window.setFramerateLimit(60);
	inputManager.addBinding("tab_confirm", sf::Keyboard::Tab);

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
	sf::Vector2f viewPos = window.mapPixelToCoords((sf::Vector2i)inputManager.getMousePosition(), graphView);

	if (inputManager.pressedOnce("cancel"))
	{
		if (selectedInputBox)
		{
			if (ibox.isSelected())
			{
				ibox.setActive(false);
				ibox.setSelected(false);
				ibox.clear();
			}
			else if (selectedNode)
			{
				// Reverting the edited textbox
				selectedInputBox->setString(selectedNode->getUnsavedInput());
			}

			selectedInputBox->setSelected(false);
			selectedInputBox = NULL;
		}

		selectedNode = NULL;
	}

	// Tab switching 
	if (inputManager.pressedOnce("tab_confirm") &&
		selectedInputBox != NULL &&
		selectedNode != NULL)
	{
		selectedNode->updateNodeText();
		selectedInputBox->setSelected(false);
		selectedInputBox = selectedNode->getOtherInputBox(selectedInputBox);
		selectedInputBox->setSelected(true);
	}

	if (inputManager.pressedOnce(sf::Mouse::Left))
	{
		if (ibox.checkClicked(viewPos))
			selectedInputBox = &ibox;
		else
		{
			for (auto i : nodeViews)
			{
				selectedInputBox = i->getSelectedInputBox(viewPos);

				if (selectedInputBox)
				{
					selectedInputBox->setSelected(true);
					break;
				}
			}
		}
	}

	while(window.pollEvent(event))
	{
		switch(event.type)
		{
			case sf::Event::TextEntered:
			{
				if(selectedInputBox)
				{
					if(event.text.unicode < 128 && event.text.unicode > 31)
					{
						selectedInputBox->addCharacter(static_cast<char>(event.text.unicode));
					}
					else if(event.text.unicode == 8) //Backspace
					{
						selectedInputBox->removeCharacter();
					}
					else if(event.text.unicode == 13) //Return
					{
						// If there's an InputBox selected
						if (selectedInputBox)
						{
							// If it's the module's inputbox
							if (ibox.isSelected())
							{
								switch (inState)
								{
									case NAME:
									{
										 moduleName.setString(ibox.getString());

										 int count = 0;
										 for (auto i : nodeViews)
										 {
											 i->setID(moduleName.getString(), count);
											 count++;
										 }
										 break;
									}
									case LOAD:
									{
										 std::string newName;
										 if (populateGraph(fileManager.loadDialogue(ibox.getString(), newName)) == 0)
											 moduleName.setString(newName);
										 else
											 std::cerr << "file " << ibox.getString() << " contained no nodes!" << std::endl;
										 break;
									}
									case SAVE:
									{
										 std::cout << "Under construction!" << std::endl;
										 break;
									}
								}

								// Clearing the box and InputState
								ibox.setSelected(false);
								ibox.setActive(false);
								ibox.clear();
								inState = NONE;
							}
							else // If it's one of the nodeView input boxes
							{
								selectedInputBox->setSelected(false);
								selectedNode->updateNodeText();

								// Deselecting
								selectedNode = NULL;
								selectedInputBox = NULL;
							}
						}
					}
				}
				break;
			}

			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.delta < 0)
				{
					scaleFactor = 0.90f;
					scale *= scaleFactor;
				}
				else if (event.mouseWheelScroll.delta > 0)
				{
					scaleFactor = 1.10f;
					scale *= scaleFactor;
				}
				else
				{
					scaleFactor = 1.f;
				}

				graphView.zoom(scaleFactor);
				break;
			
			case sf::Event::MouseButtonReleased:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					if (!selectedInputBox)
						selectedNode = NULL;

					movingView = false;

					for (auto i : buttons)
						i.second->setColor(colors["button"]);
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					// If we're making a connection
					if (connectingNodes[0] != NULL)
					{
						for (auto i : nodeViews)
						{
							if (i->getGlobalBounds().contains(viewPos)
							 && i != connectingNodes[0])
							{
								connectingNodes[1] = i;

								std::cout << "connecting " << connectingNodes[0]->getID();
								std::cout << " and " << connectingNodes[1]->getID() << std::endl;
								break;
							}
						}

						if (connectingNodes[1] == NULL)
							std::cout << "No node to connect to!" << std::endl;
						else
						{
							Connector connection(connectingNodes[0]->getNode(), connectingNodes[1]->getNode());

							if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
								std::cout << "Flags are under construction!" << std::endl;
							else
							{
								if (connectingNodes[0]->addConnector(connection, connectingNodes[1]->getInletPos()))
									std::cout << "Connection completed successfully!" << std::endl;
								else std::cout << "Connection failed!" << std::endl;
							}
						}

						// Clearing the connections
						connectingNodes[0] = NULL;
						connectingNodes[1] = NULL;
					}
				}
				break;
			}

			case sf::Event::MouseButtonPressed:
			{
				if(event.mouseButton.button == sf::Mouse::Left)
				{
					// Selecting a node
					for(auto i : nodeViews)
					{
						if(i->getGlobalBounds().contains(viewPos))
						{
							selectedNode = i;
						}
					}

					if(!selectedNode)
					{
						if(!graphBG.getGlobalBounds().contains(viewPos))
							movingView = true;
					}

					// Checking if any of the buttons have been clicked
					for(auto i : buttons)
					{
						i.second->update(&inputManager);

						if(i.second->isPressed())
						{
							i.second->setColor(colors["background"]);

							if (i.first == "exit")
								exit(0);

							// Flagging the input box as selected if needed
							if(i.first == "m.name")
							{
								ibox.setSelected(true);
								ibox.setActive(true);
								selectedInputBox = &ibox;
								inState = NAME;
							}

							if (i.first == "n.node")
							{
								sf::Vector2f spawnPos = window.mapPixelToCoords(sf::Vector2i(50,50), graphView);
								nodeViews.push_back(new NodeView(moduleName.getString(),
									nodeCount, spawnPos, font));

								// Tracking the total nodes created to prevent repeated IDs
								nodeCount++;
							}

							if (i.first == "load")
							{
								ibox.setSelected(true);
								ibox.setActive(true);
								selectedInputBox = &ibox;
								inState = LOAD;
							}

							if (i.first == "n.names")
								showNodeNames = !showNodeNames;
						}

					}
				}
				else if(event.mouseButton.button == sf::Mouse::Right)
				{
					for(auto i = nodeViews.begin(); i != nodeViews.end(); )
					{
						if((*i)->removeRequired(viewPos))
						{
							// Ensuring that nodes connected to i are disconnected cleanly
							for (auto j : nodeViews)
							{
								if (j != *i)
								{
									j->removeConnector(*i);
								}
							}

							delete *i;
							*i = NULL;

							i = nodeViews.erase(i);
						}
						else ++i;
					}
				}
				else if (event.mouseButton.button == sf::Mouse::Middle)
				{
					connectingNodes[0] = NULL;

					for (auto i : nodeViews)
					{
						if (i->getGlobalBounds().contains(viewPos))
						{
							std::cout << "Node Connection started" << std::endl;
							connectingNodes[0] = i;
							break;
						}
					}

					if (connectingNodes[0] == NULL)
						std::cout << "No nodeview found!" << std::endl;
				}
			}
			break;
		}
	}

	// Moving nodes
	if(selectedNode)
	{
		sf::Vector2f moveVec = inputManager.getMousePosition() - inputManager.getPrevMousePosition();
		moveVec *= scale;
		selectedNode->move(moveVec);
		//selectedNode->move(inputManager.getMousePosition() - inputManager.getPrevMousePosition());
	}
	else if(movingView)
	{
		graphView.move(inputManager.getPrevMousePosition() - inputManager.getMousePosition());
	}

	for(auto i : buttons)
	{
		if(i.second->isMouseOver())
			i.second->setHighlight(colors["buttonHighlight"]);
		else i.second->clearHighlight();
	}

	// Updating NodeView objects
	for (auto i : nodeViews)
	{
		i->update();
		i->updateLines(nodeViews);
	}
}

int Grapher::populateGraph(const std::vector<Node*>& nodes)
{
	if (nodes.size() == 0)
	{
		std::cerr << "no nodes in file!" << std::endl;
		return 1;
	}

	sf::Vector2f spawnPos = window.mapPixelToCoords(sf::Vector2i(50,50), graphView);
	std::list<NodeView*> openSet;
	std::vector<NodeView*> closedSet;

	// Clearing any preexisting nodeViews
	for (auto i = nodeViews.begin(); i != nodeViews.end(); )
	{
		delete *i;
		i = nodeViews.erase(i);
	}

	for (int i = 0; i < 2; ++i)
		connectingNodes[i] = NULL;

	for (auto i : nodes)
	{
		nodeViews.push_back(new NodeView(spawnPos, font, i));
	}

	for (auto i : nodeViews)
	{
		i->populateLines(nodeViews);
		i->update();
	}

	// Positioning the nodeViews correctly
	openSet.push_back(nodeViews.front());
	sf::Vector2f gap(150, 300);

	while (!openSet.empty())
	{
		std::vector<Connector>& conns = openSet.front()->getNode()->getConnections();
		int connCount = 0;

		for (auto conn : conns)
		{
			for (auto nodeView : nodeViews)
			{
				/* If the nodeview associated with conn isn't in the open or closed sets
				 * reposition the nodeView and push it to the open set
				*/
				if (conn.getEnd() == nodeView->getNode() &&
					std::find(openSet.begin(), openSet.end(), nodeView) == openSet.end() &&
					std::find(closedSet.begin(), closedSet.end(), nodeView) == closedSet.end())
				{
					nodeView->setPosition(openSet.front()->getOutletPos());
					nodeView->move(sf::Vector2f(gap.x, gap.y * connCount));
					openSet.push_back(nodeView);
					break;
				}
			}

			connCount++;
		}
		openSet.pop_front();
	}

	for (auto i : nodeViews)
		i->updateLines(nodeViews);


	nodeCount = nodeViews.size();
	return 0;
}

void Grapher::render()
{
	window.clear(sf::Color(31,31,31));

	window.setView(graphView);
	for(auto i : nodeViews)
		i->render(window, showNodeNames);

	window.setView(window.getDefaultView());

	window.draw(graphBG);
	window.draw(moduleName);
	ibox.render(window);

	for(auto i : buttons)
		i.second->draw(&window);
	window.display();
}
