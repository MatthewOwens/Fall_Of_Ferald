#include "Button.h"
#include <iostream>

Button::Button()
{
	//sprite = new sf::Sprite();
	rectShape = new sf::RectangleShape(sf::Vector2f(0.f, 0.f));
}
Button::Button(const sf::Texture &texture)
{
	sprite = new sf::Sprite(texture);
}

Button::Button(const sf::Texture &texture, const sf::IntRect &rectangle)
{
	sprite = new sf::Sprite(texture, rectangle);
}

Button::Button(const sf::Vector2f size, const sf::Color color)
{
	rectShape = new sf::RectangleShape(size);
	rectShape->setFillColor(color);
}

Button::~Button()
{
	// Cleaning everything up
	if(sprite != NULL)
	{
		delete sprite;
		sprite = NULL;
	}
	if(rectShape != NULL)
	{
		delete rectShape;
		rectShape = NULL;
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
	if(rectShape)
		window->draw(*rectShape);
	if(text)
		window->draw(*text);
}

bool Button::isPressed(InputManager* inputManager)
{
	if(inputManager->pressedOnce(sf::Mouse::Button::Left))
	{
		if(sprite)
			return sprite->getGlobalBounds()
				.contains(inputManager->getMousePosition());
		else return rectShape->getGlobalBounds()
				.contains(inputManager->getMousePosition());
	}
	else return false;	// Can't be pressed if the LMB isn't down
}

void Button::setColor(const sf::Color& color)
{
	if(sprite)
		sprite->setColor(color);

	if(rectShape)
		rectShape->setFillColor(color);
}

void Button::setTexture(const sf::Texture& texture, bool resetRect)
{
	if(sprite)
		sprite->setTexture(texture, resetRect);
}

void Button::setPosition(const sf::Vector2f& position)
{
	if(sprite)
		sprite->setPosition(position);
	if(rectShape)
		rectShape->setPosition(position);
	/*if(iconSprite)
		iconSprite->setPosition(position);*/
	updatePositions();
}

void Button::setScale(const sf::Vector2f& factors)
{
	if(sprite)
		sprite->setScale(factors);
	if(rectShape)
		rectShape->setScale(factors);
}

const sf::Vector2f& Button::getPosition()
{
	if(sprite)
		return sprite->getPosition();

	if(rectShape)
		return rectShape->getPosition();

	// Preventing things breaking
	else return sf::Vector2f(0.f, 0.f);
}

const sf::Vector2f& Button::getScale()
{
	if(sprite)
		return sprite->getScale();

	if(rectShape)
		return rectShape->getScale();
}

void Button::rotate(float angle)
{
	if(sprite)
		sprite->rotate(angle);
	if(rectShape)
		rectShape->rotate(angle);
}

void Button::move(const sf::Vector2f& offset)
{
	if(sprite)
		sprite->move(offset);
	if(rectShape)
		rectShape->move(offset);

	updatePositions();
}

void Button::scale(const sf::Vector2f& factor)
{
	if(sprite)
		sprite->scale(factor);
	if(rectShape)
		rectShape->scale(factor);
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

void Button::updatePositions()
{
	sf::Vector2f pos = getPosition();
	sf::FloatRect buttonBounds;

	if(sprite)
		buttonBounds = sprite->getLocalBounds();
	else buttonBounds = rectShape->getLocalBounds();

	// Centering the text
	if(text)
	{
		sf::FloatRect textBounds = text->getLocalBounds();
		text->setPosition(pos.x + (buttonBounds.width / 2) - (textBounds.width / 2),
						pos.y + (buttonBounds.height / 2) - textBounds.height);
	}
}
