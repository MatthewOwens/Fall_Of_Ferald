#include "DialogueBox.h"
#include <SFML/Graphics.hpp>

DialogueBox::DialogueBox(const sf::Texture& texture, const sf::Font& font,
					const int charSize, const int posX, const int posY)
{
	sprite.setPosition(posX, posY);
	sprite.setTexture(texture);

	name.setFont(font);
	dialogue.setFont(font);
	continuePrompt.setFont(font);

	name.setCharacterSize(charSize);
	dialogue.setCharacterSize(charSize);
	continuePrompt.setCharacterSize(charSize*0.6);

	name.setPosition(sprite.getPosition());
	name.move(25, 10);

	dialogue.setPosition(sprite.getPosition());
	dialogue.move(25, 50);

	continuePrompt.setPosition(sprite.getPosition());
	continuePrompt.move(1100, 100);

	continuePulse = 255;
	continuePulseDir = false;
}

DialogueBox::~DialogueBox()
{
}

void DialogueBox::setStrings(const std::string& header, const std::string& body)
{
	name.setString(header);
	dialogue.setString(body);
	continuePrompt.setString("Click to continue");
}

void DialogueBox::draw(sf::RenderWindow *window)
{
	if (continuePulseDir)continuePulse+=2;
	else continuePulse-=2;

	if (continuePulse >= 255 || continuePulse <= 60)continuePulseDir = !continuePulseDir;
	continuePrompt.setColor(sf::Color(255, 255, 255, continuePulse));

	window->draw(sprite);
	//window->draw(name);
	window->draw(dialogue);
	window->draw(continuePrompt);
}
