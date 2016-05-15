#include "InputBox.h"

InputBox::InputBox(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font)
{
	baseBox = sf::RectangleShape(size);
	baseBox.setOutlineThickness(3);
	baseBox.setFillColor(sf::Color::White);
	baseBox.setOutlineColor(sf::Color::White);
	baseBox.move(position);

	text = sf::Text("", font, 20);
	text.setColor(sf::Color::Black);
	text.setPosition(baseBox.getPosition());
	selected = false;
	active = true;
}

InputBox::~InputBox()
{
}

void InputBox::addCharacter(const char& character)
{
	const std::string& str = text.getString();
	text.setString(str + character);
}

void InputBox::addCharacter(const std::string& character)
{
	const std::string& str = text.getString();
	text.setString(str + character);
}

void InputBox::removeCharacter()
{
	std::string str = text.getString();
	if(str.size() == 0)
		return;

	str = str.erase(str.size() - 1);
	text.setString(str);
}

void InputBox::checkClicked(const sf::Vector2f& mousePos)
{
	selected = baseBox.getGlobalBounds().contains(mousePos);

	if(selected)
	{
		baseBox.setOutlineColor(sf::Color(179,80,80));

	}
	else
		baseBox.setOutlineColor(sf::Color::White);
}

void InputBox::render(sf::RenderWindow& window)
{
	if(active)
	{
		window.draw(baseBox);
		window.draw(text);
	}
}

void InputBox::setSelected(bool val)
{
	if(val)
		baseBox.setOutlineColor(sf::Color(179,80,80));
	else
		baseBox.setOutlineColor(sf::Color::White);

	selected = val;
}
