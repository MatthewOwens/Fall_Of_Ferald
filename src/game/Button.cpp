#include "Button.h"
#include <iostream>

Button::Button()
{
	sprite = new sf::Sprite();
}

Button::Button(const sf::Texture &texture)
{
	sprite = new sf::Sprite(texture);
}

Button::Button(const sf::Texture &texture, const sf::IntRect &rectangle)
{
	sprite = new sf::Sprite(texture, rectangle);
}

Button::~Button()
{
	// Cleaning everything up
	if(sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
	if(iconSprite != NULL)
	{
		delete iconSprite;
		iconSprite = NULL;
	}
	if(text != NULL)
	{
		delete text;
		text = NULL;
	}
}

void Button::draw(sf::RenderWindow* window)
{
	if(sprite)
		window->draw(*sprite);
	if(iconSprite)
		window->draw(*iconSprite);
	if(text)
		window->draw(*text);
}

bool Button::isPressed(InputManager* inputManager)
{
	if(inputManager->pressedOnce(sf::Mouse::Button::Left))
	{
		return sprite->getGlobalBounds()
			.contains(inputManager->getMousePosition());
	}
	else return false;	// Can't be pressed if the LMB isn't down
}

void Button::setColor(const sf::Color& color)
{
	if(sprite)
		sprite->setColor(color);
}

void Button::setTexture(const sf::Texture& texture, bool resetRect)
{
	if(sprite)
		sprite->setTexture(texture, resetRect);
	if(iconSprite)
		iconSprite->setTexture(texture, resetRect);
}

void Button::setPosition(const sf::Vector2f& position)
{
	if(sprite)
		sprite->setPosition(position);
	/*if(iconSprite)
		iconSprite->setPosition(position);*/
	updatePositions();
}

void Button::setScale(const sf::Vector2f& factors)
{
	if(sprite)
		sprite->setScale(factors);
	if(iconSprite)
		iconSprite->setScale(factors);
}

const sf::Vector2f& Button::getPosition()
{
	if(sprite)
		return sprite->getPosition();
}

const sf::Vector2f& Button::getScale()
{
	if(sprite)
		return sprite->getScale();
}

void Button::rotate(float angle)
{
	if(sprite)
		sprite->rotate(angle);
	if(iconSprite)
		iconSprite->rotate(angle);
}

void Button::move(const sf::Vector2f& offset)
{
	if(sprite)
		sprite->move(offset);
	/*if(iconSprite)
		iconSprite->move(offset);
	if(text)
		text->move(offset);*/
	updatePositions();
}

void Button::scale(const sf::Vector2f& factor)
{
	if(sprite)
		sprite->scale(factor);
	if(iconSprite)
		iconSprite->scale(factor);
}

void Button::setText(const sf::String &string, const sf::Font &font, unsigned int characterSize)
{
	if(!text)
		text = new sf::Text(string, font, characterSize);
	else	// No need to call new again if text already exists
	{
		text->setString(string);
		text->setFont(font);
		text->setCharacterSize(characterSize);
	}

	updatePositions();
}

void Button::setIconTexture(const sf::Texture& texture, bool resetRect)
{
	if(iconSprite)
	{
		iconSprite->setTexture(texture, resetRect);
		updatePositions();
	}
	else
		std::cout << "\tError: iconSprite is null, please use an alternative Button::setIconTexture" << std::endl;
}

void Button::setIcon(const sf::Texture& texture)
{
	if(iconSprite)
	{
		delete iconSprite;
		iconSprite = NULL;
	}

	iconSprite = new sf::Sprite(texture);
	updatePositions();
}

void Button::setIcon(const sf::Texture& texture, const sf::IntRect& rectangle)
{
	if(iconSprite)
	{
		delete iconSprite;
		iconSprite = NULL;
	}

	iconSprite = new sf::Sprite(texture, rectangle);
	updatePositions();
}

void Button::updatePositions()
{
	sf::Vector2f pos = getPosition();
	sf::FloatRect buttonBounds = sprite->getLocalBounds();

	// Centering the iconSprite
	if(iconSprite)
	{
		sf::FloatRect iconBounds = iconSprite->getLocalBounds();

		iconSprite->setPosition(pos.x + (buttonBounds.width / 2) - (iconBounds.width / 2),
						pos.y + (buttonBounds.height / 2) - (iconBounds.height / 2));
	}

	// Centering the text
	if(text)
	{
		sf::FloatRect textBounds = text->getLocalBounds();
		text->setPosition(pos.x + (buttonBounds.width / 2) - (textBounds.width / 2),
						pos.y + (buttonBounds.height / 2) - (textBounds.height / 2));
	}
}
