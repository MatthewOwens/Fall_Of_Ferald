#include "ConnectionEditor.h"

ConnectionEditor::ConnectionEditor(std::vector<Connector>& connections, sf::Vector2f spawnPos, const sf::Font& font)
:conns(connections)
{
	sf::Vector2f stringBoxSize = sf::Vector2f(245, 25);
	sf::Vector2f prioBoxSize = sf::Vector2f(25, 25);
	sf::Vector2f paddingSize = sf::Vector2f(10, 10);

	for (auto i : conns)
	{
		sf::Vector2f prioSpawnPos = spawnPos;
		prioSpawnPos.x += (stringBoxSize.x + paddingSize.x);

		// string text box
		iboxes.push_back(InputBox(spawnPos, stringBoxSize, font));
		iboxes.back().setString(i.getChoiceText());
		
		// priority text box
		iboxes.push_back(InputBox(prioSpawnPos, prioBoxSize, font));
		iboxes.back().setString(std::to_string(i.getPriority()));

		spawnPos.y += (paddingSize.y + stringBoxSize.y);
	}
	selectedIndex = -1;
}

void ConnectionEditor::addCharacter(char in)
{
	if (selectedIndex == -1)
		return;
	else iboxes[selectedIndex].addCharacter(in);
	std::cout << "iboxes[" << selectedIndex << "] : " << iboxes[selectedIndex].getString() << std::endl;
}

void ConnectionEditor::removeCharacter()
{
	if (selectedIndex == -1)
		return;
	else iboxes[selectedIndex].removeCharacter();
}

void ConnectionEditor::confirmEdits()
{
	if (selectedIndex == -1)
		return;

	else if (selectedIndex % 2 == 0)	// string text box
		conns[selectedIndex].setChoiceText(iboxes[selectedIndex].getString());
	else	// priority text box
		conns[selectedIndex / 2].setPriority(std::stoi(iboxes[selectedIndex].getString()));
}

void ConnectionEditor::cancelEdits()
{
	if (selectedIndex == -1)
		return;
	else if (selectedIndex % 2 == 0)	// string text box
		iboxes[selectedIndex].setString(conns[selectedIndex].getChoiceText());
	else	// priority text box
		iboxes[selectedIndex].setString(std::to_string(conns[selectedIndex].getPriority()));
}

void ConnectionEditor::updateSelection(const sf::Vector2f& mousePos)
{
	selectedIndex = -1;

	for (int i = 0; i < iboxes.size(); ++i)
	{
		if (iboxes[i].checkClicked(mousePos))
		{
			selectedIndex = i;
		}
	}
}

void ConnectionEditor::render(sf::RenderWindow& window)
{
	for (auto i : iboxes)
		window.draw(i);
}

ConnectionEditor::~ConnectionEditor()
{
}
