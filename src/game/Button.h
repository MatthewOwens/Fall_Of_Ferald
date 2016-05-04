/// Simple class that extends SFML's sprite class with a few methods to check
/// when the sprite has been clicked

#ifndef BUTTON_H
#define BUTTON_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>
#include "InputManager.h"

class Button 
{
public:
	Button();
	Button(const sf::Vector2f size, const sf::Color color = sf::Color(255, 255, 255));
	Button(const sf::Texture &texture);
	Button(const sf::Texture &texture, const sf::IntRect &rectangle);
	~Button();
	bool isPressed(InputManager* inputManager);

	void setTexture(const sf::Texture &texture, bool resetRect = false);
	void setIconTexture(const sf::Texture& texture, bool resetRect = false);
	void setPosition(const sf::Vector2f &position);
	void setScale(const sf::Vector2f &factors);
	void setText(const sf::String &string, const sf::Font &font, unsigned int characterSize=20);
	void setColor(const sf::Color& color);

	const sf::Vector2f& getPosition();
	const sf::Vector2f& getScale();

	void rotate(float angle);
	void move(const sf::Vector2f& offset);
	void scale(const sf::Vector2f& factor);

	void draw(sf::RenderWindow* window);
private:
	sf::Sprite* sprite = NULL;
	sf::RectangleShape* rectShape = NULL;
	//sf::Sprite* iconSprite = NULL;
	sf::Text* text = NULL;

	void updatePositions();
};
#endif//BUTTON_H
