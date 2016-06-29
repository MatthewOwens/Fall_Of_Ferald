#ifndef CONNECTIONEDITOR_H
#define CONNECTIONEDITOR_H
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Connector.h"
#include "InputBox.h"
#include <iostream>
#include <vector>

class ConnectionEditor
{
public:
	ConnectionEditor(std::vector<Connector>& connections, sf::Vector2f spawnPos, const sf::Font& font);
	~ConnectionEditor();

	void render(sf::RenderWindow& window);
	void updateSelection(const sf::Vector2f& mousePos);

	void addCharacter(char in);
	void removeCharacter();
	void confirmEdits();
	void cancelEdits();
private:
	std::vector<Connector>& conns;
	std::vector<InputBox> iboxes;
	int selectedIndex = -1;
};
#endif//CONNECTIONEDITOR_H
