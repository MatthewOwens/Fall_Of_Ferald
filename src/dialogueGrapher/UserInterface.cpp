#include "UserInterface.h"

UserInterface::UserInterface()
{
	// Defining the UI colors
	colors["buttonPressed"] = sf::Color::White;
	colors["button"] = sf::Color(142, 196, 137);

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
		if(i.second->isPressed(inputManager))
			i.second->setColor(colors["buttonPressed"]);
		else 
			i.second->setColor(colors["button"]);
	}
}

void UserInterface::render(sf::RenderWindow& window)
{
	for(auto i : buttons)
		i.second->draw(&window);
}
