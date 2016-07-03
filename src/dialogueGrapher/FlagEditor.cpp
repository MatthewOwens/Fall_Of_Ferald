#include "FlagEditor.h"

FlagEditor::FlagEditor(Connector& connection, std::map<std::string, bool>& local,
						std::map<std::string, bool>& global, sf::Vector2f availableSize,
						const sf::Font& font)
:conn(connection),
localFlags(local),
globalFlags(global),
rect(sf::RectangleShape(availableSize))
{
	sf::Vector2f textSpawn = sf::Vector2f(50, 50);
	sf::Vector2f padding = sf::Vector2f(20, 0);

	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(42, 42, 42));
	float textHeight = 0.f;
	float textLength = 0.f;

	breakTexts[0] = sf::Text("Local Flags:", font, charSize);
	breakTexts[1] = sf::Text("Global Flags:", font, charSize);
	breakTexts[2] = sf::Text("Required Flags:", font, charSize);
	breakTexts[3] = sf::Text("Triggered Flags:", font, charSize);

	breakTexts[0].setPosition(40, 40);

	for (auto i : localFlags)
	{
		localTexts.push_back(sf::Text(i.first, font, charSize));
		localTexts.back().setPosition(textSpawn);

		textHeight = localTexts.back().getGlobalBounds().height;
		textLength = localTexts.back().getGlobalBounds().width;
		
		if (i.second)
			localTexts.push_back(sf::Text("true", font, charSize));
		else
			localTexts.push_back(sf::Text("false", font, charSize));

		localTexts.back().setPosition(textSpawn + padding);
		localTexts.back().move(textLength, 0);

		// Moving the spawn
		textSpawn.y += padding.x + textHeight;
	}

	textSpawn.y += padding.x * 2;
	breakTexts[1].setPosition(textSpawn);
	breakTexts[1].move(-10, 0);
	textHeight = breakTexts[1].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : globalFlags)
	{
		globalTexts.push_back(sf::Text(i.first, font, charSize));
		globalTexts.back().setPosition(textSpawn);

		textLength = globalTexts.back().getGlobalBounds().width;
		
		if (i.second)
			globalTexts.push_back(sf::Text("true", font, charSize));
		else
			globalTexts.push_back(sf::Text("false", font, charSize));

		globalTexts.back().setPosition(textSpawn + padding);
		globalTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;
	}

	textSpawn.y += padding.x * 2;
	breakTexts[2].setPosition(textSpawn);
	breakTexts[2].move(-10, 0);
	textHeight = breakTexts[2].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (auto i : conn.getFlags())
	{
		requiredTexts.push_back(sf::Text(i.first, font, charSize));
		requiredTexts.back().setPosition(textSpawn);

		textLength = requiredTexts.back().getGlobalBounds().width;
		
		if (i.second)
			requiredTexts.push_back(sf::Text("true", font, charSize));
		else
			requiredTexts.push_back(sf::Text("false", font, charSize));

		requiredTexts.back().setPosition(textSpawn + padding);
		requiredTexts.back().move(textLength, 0);

		textSpawn.y += padding.x + textHeight;
	}
	
	breakTexts[3].setPosition(textSpawn);
	breakTexts[3].move(-10, 0);
	textHeight = breakTexts[3].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;
}

void FlagEditor::render(sf::RenderWindow& window)
{
	window.draw(rect);

	for (auto i : localTexts)
		window.draw(i);

	for (auto i : globalTexts)
		window.draw(i);

	for (auto i : requiredTexts)
		window.draw(i);

	for (auto i : triggeredTexts)
		window.draw(i);

	for (auto i : breakTexts)
		window.draw(i);
}

FlagEditor::~FlagEditor()
{
}
