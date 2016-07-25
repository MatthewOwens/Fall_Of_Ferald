#include "DialogueBox.h"
#include <SFML/Graphics.hpp>

DialogueBox::DialogueBox(const sf::Texture& texture, const sf::Font font,
					const int charSize = 48, const int posX = 0, const int posY = 0)
{
	sprite.setPosition(posX, posY);
	sprite.setTexture(texture);

	name.setFont(font);
	dialogue.setFont(font);

	name.setCharacterSize(charSize);
	dialogue.setCharacterSize(charSize);
}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::setStrings(const std::string& header, const std::string& body)
{
	name.setString(header);
	dialogue.setString(body);
}

void DialogueBox::draw(sf::RenderWindow *window)
{
	window->draw(sprite);
	window->draw(name);
	window->draw(dialogue);
}
