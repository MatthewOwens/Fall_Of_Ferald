#include "InputBox.h"

InputBox::InputBox(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font)
: RectangleShape(size)
{
	setOutlineThickness(3);
	setFillColor(sf::Color::White);
	setOutlineColor(sf::Color::White);
	//move(position);
	setPosition(position);

	text = sf::Text("", font, 20);
	text.setColor(sf::Color::Black);
	text.setPosition(position);
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
	selected = getGlobalBounds().contains(mousePos);

	if(selected)
	{
		setOutlineColor(sf::Color(179,80,80));

	}
	else
		setOutlineColor(sf::Color::White);
}

void InputBox::render(sf::RenderWindow& window)
{
	if(active)
	{
		window.draw(*this);
		window.draw(text);
	}
}

void InputBox::setSelected(bool val)
{
	if(val)
		setOutlineColor(sf::Color(179,80,80));
	else
		setOutlineColor(sf::Color::White);

	selected = val;
}
