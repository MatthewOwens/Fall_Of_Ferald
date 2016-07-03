#include "FlagEditor.h"
#include <locale>

FlagEditor::FlagEditor(Connector& connection, std::map<std::string, bool>& local,
	std::map<std::string, bool>& global, sf::Vector2f availableSize,
	const sf::Font& font, const sf::Texture& buttonTexture)
	:conn(connection),
	localFlags(local),
	globalFlags(global),
	rect(sf::RectangleShape(availableSize)),
	fnt(font)
{
	sf::Vector2f textSpawn = sf::Vector2f(50, 50);
	sf::Vector2f padding = sf::Vector2f(20, 0);

	rect.setPosition(0, 0);
	rect.setFillColor(sf::Color(29,29,29));
	float textHeight = 0.f;
	float textLength = 0.f;
	clickedButton = -1;

	breakTexts[LOCAL] = sf::Text("Local Flags:", font, charSize);
	breakTexts[GLOBAL] = sf::Text("Global Flags:", font, charSize);
	breakTexts[REQUIRED] = sf::Text("Required Flags:", font, charSize);
	breakTexts[TRIGGERED] = sf::Text("Triggered Flags:", font, charSize);

	inStrings[0] = "";
	inStrings[1] = "";

	breakTexts[LOCAL].setPosition(40, 40);

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
	breakTexts[GLOBAL].setPosition(textSpawn);
	breakTexts[GLOBAL].move(-10, 0);
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
	breakTexts[REQUIRED].setPosition(textSpawn);
	breakTexts[REQUIRED].move(-10, 0);
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
	
	breakTexts[TRIGGERED].setPosition(textSpawn);
	breakTexts[TRIGGERED].move(-10, 0);
	textHeight = breakTexts[3].getGlobalBounds().height;
	textSpawn.y += padding.x + textHeight;

	for (int i = 0; i < 4; ++i)
	{
		buttons[i] = new Button(buttonTexture);
		buttons[i]->setPosition(breakTexts[i].getPosition());
		buttons[i]->move(sf::Vector2f(500, 0));
	}
}
	
void FlagEditor::moveTextBlock(TextBlocks block, float moveVal)
{
	switch (block)
	{
		case LOCAL:
			breakTexts[LOCAL].move(0, moveVal);
			for (auto &i : localTexts)
				i.move(0, moveVal);
		case GLOBAL:
			breakTexts[GLOBAL].move(0, moveVal);
			for (auto &i : globalTexts)
				i.move(0, moveVal);
		case REQUIRED:
			breakTexts[REQUIRED].move(0, moveVal);
			for (auto &i : requiredTexts)
				i.move(0, moveVal);
		case TRIGGERED:
			breakTexts[TRIGGERED].move(0, moveVal);
			for (auto &i : triggeredTexts)
				i.move(0, moveVal);
	}
}

// Returns true if a button was pressed, false otherwise
bool FlagEditor::checkButtons(InputManager* inputManager)
{
	clickedButton = -1;
	for (int i = 0; i < 4; ++i)
	{
		buttons[i]->update(inputManager);
		if (buttons[i]->isPressed())
			clickedButton = i;
	}

	if (clickedButton > -1 && clickedButton < 3)
	{
		moveTextBlock((TextBlocks)(clickedButton + 1), 50.f);
		return true;
	}
	else return false;
}

void FlagEditor::checkText(const sf::Vector2f& mousePos)
{
	clicked(mousePos, requiredTexts);
	clicked(mousePos, triggeredTexts);
}

void FlagEditor::clicked(const sf::Vector2f& mousePos, std::vector<sf::Text>& vec)
{
	sf::Color color;

	for (int i = 0; i < vec.size(); ++i)
	{
		if (vec[i].getGlobalBounds().contains(mousePos))
		{
			if (vec[i].getColor() == sf::Color::White)
				color = sf::Color(142, 196, 137);
			else color = sf::Color::White;

			if (i % 2 == 1)	// Clicked on the value of the flag
			{
				vec[i - 1].setColor(color);

				if (vec[i].getString() == "false")
					vec[i].setString("true");
				else vec[i].setString("false");
			}
			else
			{
				vec[i + 1].setColor(color);

				if (vec[i + 1].getString() == "false")
					vec[i + 1].setString("true");
				else vec[i + 1].setString("false");
			}

			vec[i].setColor(color);
		}
	}
}

void FlagEditor::getString(std::string str)
{
	bool flagVal = false;

	if (inStrings[0] == "")
	{
		inStrings[0] = str;
		return;
	}
	else
	{
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);

		if (str == "false")
		{
			inStrings[1] = str;
			flagVal = false;
		}
		else if (str == "true")
		{
			inStrings[1] = str;
			flagVal = true;
		}
		else
		{
			inStrings[0] = "";
			inStrings[1] = "";
			return;
		}
	}
	
	// Both inStrings populated with valid values
	conn.addFlag(inStrings[0], flagVal);

	if ((TextBlocks)clickedButton == REQUIRED)
	{
		addText(requiredTexts);
	}
	else if ((TextBlocks)clickedButton == TRIGGERED)
	{
		addText(triggeredTexts);
	}

	inStrings[0] = "";
	inStrings[1] = "";
}

void FlagEditor::addText(std::vector<sf::Text>& vec)
{
	auto ref = breakTexts[clickedButton];
	int count = vec.size();

	vec.push_back(sf::Text(inStrings[0], fnt, charSize));
	vec.back().setPosition(ref.getPosition());
	if (count != 0)
		vec.back().move(10, vec[count-1].getGlobalBounds().height + 20);
	else vec.back().move(10, ref.getGlobalBounds().height + 20);

	vec.push_back(sf::Text(inStrings[1], fnt, charSize));
	vec.back().setPosition(ref.getPosition());
	if (count != 0)
		vec.back().move(10, vec[count-1].getGlobalBounds().height + 20);
	else vec.back().move(10, ref.getGlobalBounds().height + 20);
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

	for (auto i : buttons)
		i->draw(&window);
}

FlagEditor::~FlagEditor()
{
	for (int i = 0; i < 4; ++i)
	{
		delete buttons[i];
		buttons[i] = NULL;
	}
}