#include "UserInterface.h"

UserInterface::UserInterface()
{
	// Defining the UI colors
	colors["button"] = sf::Color(142, 196, 137);
	colors["buttonHighlight"] = sf::Color(179,80,80);
	colors["graphBG"] = sf::Color(31,31,31);
	colors["background"] = sf::Color(43,43,43);

	// Loading the font
	font.loadFromFile("assets/fonts/EaseOfUse.ttf");

	// Initilising the buttons
	buttons["test"] = new Button(sf::Vector2f(50,20), colors["button"]);
	buttons["test"]->setPosition(sf::Vector2f(100, 100));

	// Setting the button text
	for(auto i : buttons)
		i.second->setText(i.first, font);
}

UserInterface::~UserInterface()
{
	colors.clear();
	buttons.clear();
}

void UserInterface::update(InputManager* inputManager)
{
	// Checking if any of the buttons have been pressed
	for(auto i : buttons)
	{
		/*if(i.second->isHeld(inputManager))
			i.second->setColor(colors["background"]);
		else 
			i.second->setColor(colors["button"]);*/
		i.second->update(inputManager);

		if(i.second->isHeld() || i.second->isPressed())
			i.second->setColor(colors["background"]);
		else i.second->setColor(colors["button"]);

		if(i.second->isMouseOver())
			i.second->setHighlight(colors["buttonHighlight"]);
		else i.second->clearHighlight();
	}
}

void UserInterface::render(sf::RenderWindow& window)
{
	for(auto i : buttons)
		i.second->draw(&window);
}
