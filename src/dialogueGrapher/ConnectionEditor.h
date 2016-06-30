#ifndef CONNECTIONEDITOR_H
#define CONNECTIONEDITOR_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Connector.h"
#include "InputBox.h"
#include "NodeView.h"
#include <iostream>
#include <vector>

class ConnectionEditor
{
public:
	ConnectionEditor(NodeView* target, sf::Vector2f spawnPos, const sf::Font& font);
	~ConnectionEditor();

	void render(sf::RenderWindow& window);
	void updateSelection(const sf::Vector2f& mousePos);

	void updateText(int unicode);
	void cancelEdits();
	inline bool selectionExists() { return (selectedIndex == -1 ? false : true); }
private:
	std::vector<Connector>& conns;
	std::vector<sf::Text>& prioTexts;
	std::vector<sf::Text>& choiceTexts;

	std::vector<InputBox> iboxes;
	int selectedIndex = -1;

	void addCharacter(char in);
	void removeCharacter();
	void confirmEdits();
};
#endif//CONNECTIONEDITOR_H
