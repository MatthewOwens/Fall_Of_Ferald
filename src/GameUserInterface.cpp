#include "GameUserInterface.h"
#include <SFML/Graphics/Image.hpp>
#include <iostream>

GameUserInterface::GameUserInterface()
{
/*
    // Loading in the fonts required for the UI
    regularFont.loadFromFile("assets/fonts/steelfish rg.ttf");
    italicFont.loadFromFile ("assets/fonts/steelfish rg it.ttf");

	tooltipTexture = tooltipImage;
*/
}

void GameUserInterface::loadAssets(ImageManager& imageManager)
{
    // Loading in the fonts required for the UI
    regularFont.loadFromFile("assets/fonts/steelfish rg.ttf");
    italicFont.loadFromFile ("assets/fonts/steelfish rg it.ttf");

	// Loading the textures
	//imageManager.loadImage("assets/images/interface/tooltip.png", "tooltip");
	imageManager.loadImage("assets/images/interface/newTooltip.png", "newTooltip");

	//tooltipTexture = &imageManager.getTexture("tooltip");
	tooltipTexture= &imageManager.getTexture("newTooltip");
	tooltipTexture->setRepeated(true);
}

GameUserInterface::~GameUserInterface()
{
    //dtor
}

void GameUserInterface::update(sf::Vector2f pointerPosition, sf::Vector2f prevPointerPos, bool lmbPressed, bool rmbPressed)
{
    int offsetX, offsetY;       // The x and y offset to move the tooltip by

	// If the LMB is _held_ not pressed
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		for(auto itr = tooltips.begin(); itr != tooltips.end(); ++itr)
		{
			if(itr->containsPoint(pointerPosition))
			{
				offsetX = pointerPosition.x - prevPointerPos.x;
				offsetY = pointerPosition.y - prevPointerPos.y;
				itr->sprite.move(offsetX, offsetY);
				itr->headerText.move(offsetX, offsetY);
				itr->bodyText.move(offsetX, offsetY);
				break;
			}
		}
	}
	else if (rmbPressed)
	{
		for(auto itr = tooltips.begin(); itr != tooltips.end(); )
		{
			if(itr->containsPoint(pointerPosition))
			{
				itr = tooltips.erase(itr);
				break;
			}
			else ++itr;
		}
	}
}

// Method to add a new tooltip to the tooltip list
/*void GameUserInterface::addTooltip(std::string header, std::string body, int x, int y)
{
    // Temporary object to store the values in
    Tooltip tempTooltip;

    // Creating the header text
    tempTooltip.headerText.setString(header);
    tempTooltip.headerText.setFont(regularFont);
    tempTooltip.headerText.setPosition(x + 5, y + 1);
    tempTooltip.headerText.setScale(.75f, .75f);

    // Creating the body text
    tempTooltip.bodyText.setString(body);
    tempTooltip.bodyText.setFont(regularFont);
    tempTooltip.bodyText.setPosition(x + 12, y + 25);
    tempTooltip.bodyText.setScale(.75f, .75f);

    tempTooltip.sprite.setPosition(x, y);
    tempTooltip.isCloseable = true;
    tempTooltip.isMoveable = true;
    tempTooltip.sprite.scale(0.5f, 0.5f);

	if(tooltipTexture != NULL)		// Preventing possible null pointer dereferencing
    	tempTooltip.sprite.setTexture(*tooltipTexture);
	else std::cout << "Tooltip texture not loaded!" << std::endl;

    // Adding the temporary object to the tooltip list
    tooltips.push_back(tempTooltip);
}*/

void GameUserInterface::addTooltip(std::string header, std::string body, int x, int y, int characterSize)
{
	int lineCount = 0;	// Number of line in the text
	float scaleFactorY = 0;
	Tooltip tempTooltip;
	sf::Image spriteSheet;

	// Counting how many lines we'll need
	for(auto itr : body)
	{
		if(itr == '\n')
			lineCount++;
	}

	// Building the tooltip
	// Creating the header text
	tempTooltip.headerText.setString(header);
	tempTooltip.headerText.setFont(regularFont);
	tempTooltip.headerText.setPosition(x + 5, y + 1);
	//tempTooltip.headerText.setScale(.75f, .75f);
	tempTooltip.headerText.setCharacterSize(characterSize);

	// Creating the body text
	tempTooltip.bodyText.setString(body);
	tempTooltip.bodyText.setFont(regularFont);
	tempTooltip.bodyText.setPosition(x + 12, y + 25);
	tempTooltip.bodyText.setCharacterSize(characterSize);

	tempTooltip.sprite.setPosition(x, y);
	tempTooltip.isCloseable = true;
	tempTooltip.isMoveable = true;
	//tempTooltip.sprite.scale(0.5f, 0.5f);

	if(tooltipTexture != NULL)// Preventing possible null pointer dereferencing
	{
		// Setting the texture
		tempTooltip.sprite.setTexture(*tooltipTexture);

		// Ensuring proper scaling for the font
		scaleFactorY = tempTooltip.bodyText.getLocalBounds().height / tempTooltip.sprite.getLocalBounds().height;
		scaleFactorY += 0.1f;	// TODO: Replace magic number with scaling to include decenders
		tempTooltip.sprite.setScale(1, scaleFactorY);

		// Ensuring proper scaling for the number of lines
		sf::Vector2u textureSize = tooltipTexture->getSize();
		//tempTooltip.sprite.scale(1, lineCount);
	}
	else std::cout << "Tooltip texture not loaded!" << std::endl;

	// Pushing to the tooltip list
		tooltips.push_back(tempTooltip);
}

// Method to draw the various UI elements to the screen
void GameUserInterface::draw(sf::RenderWindow *window, sf::View& camera)
{
	// Ensuring that the tile highlights are bound to the camera
	window->setView(camera);

	// Drawing the highlights
	for (auto i = highlights.begin(); i != highlights.end(); i++)
		i->draw(window);

	// Drawing the tooltips
    for (auto i = tooltips.begin(); i != tooltips.end(); i++)
        i->draw(window);

	// Making the rest of the interface static
	window->setView(window->getDefaultView());

	// Resetting the view
	window->setView(camera);
}

// Method to get the tooltip list
std::list<Tooltip> GameUserInterface::getTooltips() { return tooltips; }

// Method to return a specific tooltip based on it's header
Tooltip GameUserInterface::getTooltip(std::string header)
{
    std::list<Tooltip>::iterator i;
    for (i = tooltips.begin(); i != tooltips.end(); i++)
        if (i->headerText.getString() == header)
            return *i;

    std::cout << "Tooltip with header '" << header << "'could not be found." << std::endl;
    return Tooltip();
}

// Method to return a specific tooltip based on the mouse pointer
Tooltip GameUserInterface::getTooltip(int mouseX, int mouseY)
{
    std::list<Tooltip>::iterator i;
    for (i = tooltips.begin(); i != tooltips.end(); i++)
        if (i->sprite.getGlobalBounds().contains(mouseX, mouseY))
            return *i;

    std::cout << "Tooltip at (" << mouseX << "," << mouseY << ") could not be found." << std::endl;
    return Tooltip();
}

void GameUserInterface::highlightTiles(std::vector<sf::Vector2i> highlightRange, sf::Color colour, int tileSize)
{
	for(auto itr = highlightRange.begin(); itr != highlightRange.end(); ++itr)
	{
		highlights.push_back(TileHighlight(itr->x, itr->y, colour, tileSize));
	}
}

void GameUserInterface::highlightTiles(std::vector<sf::Vector3i> highlightRange, sf::Color colour, int tileSize)
{
	for(auto itr = highlightRange.begin(); itr != highlightRange.end(); ++itr)
	{
		highlights.push_back(TileHighlight(itr->x, itr->y, colour, tileSize));
	}
}

void GameUserInterface::highlightTiles(std::stack<sf::Vector2i> highlightRange, sf::Color colour, int tileSize)
{
	while(!highlightRange.empty())
	{
		sf::Vector2i& current = highlightRange.top();
		highlights.push_back(TileHighlight(current.x, current.y, colour, tileSize));
		highlightRange.pop();
	}
}

void GameUserInterface::highlightTile(int x, int y, sf::Color colour, int tileSize)
{
	highlights.push_back(TileHighlight(x, y, colour, tileSize));
}

// Clearing highlights of a certain colour
void GameUserInterface::clearHighlight(sf::Color colour)
{
	//for(int i = 0; i < highlights.size() ; )
	//std::vector<TileHighlight>::iterator i;
	for(auto i = highlights.begin(); i != highlights.end() ; )
	{
		if(i->getColour() == colour)
			i = highlights.erase(i);
		else i++;
	}
}
