#ifndef FLAGEDITOR_H
#define FLAGEDITOR_H
#include "InputBox.h"
#include "Connector.h"
#include <map>
#include <string>
#include <SFML/Graphics/Text.hpp>

#ifndef _WIN32
#include "Button.h"
#else
#include "../game/Button.h"
#endif

class FlagEditor
{
public:
	FlagEditor(Connector& conn, std::map<std::string, bool>& localFlags,
		std::map<std::string, bool>& globalFlags, sf::Vector2f availableSize,
		const sf::Font& font);

	~FlagEditor();
	void render(sf::RenderWindow& window);
private:
	Connector& conn;	// The connection who's flags we're going to be editing
	std::map<std::string, bool> &localFlags;
	std::map<std::string, bool> &globalFlags;

	std::vector<sf::Text> requiredTexts;
	std::vector<sf::Text> triggeredTexts;
	std::vector<sf::Text> localTexts;
	std::vector<sf::Text> globalTexts;

	sf::Text breakTexts[4];
	const unsigned int charSize = 20;

	sf::RectangleShape rect;
};
#endif//FLAGEDITOR_H
