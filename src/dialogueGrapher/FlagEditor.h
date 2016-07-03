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
		const sf::Font& font, const sf::Texture& buttonTexture);

	~FlagEditor();
	void render(sf::RenderWindow& window);
	bool checkButtons(InputManager* inputManager);
	void checkText(const sf::Vector2f& mousePos);
	void getString(std::string str);
	inline bool gettingText() { return (inStrings[0] == "" ? false : true); }
private:
	enum TextBlocks {LOCAL = 0, GLOBAL, REQUIRED, TRIGGERED};

	Connector& conn;	// The connection who's flags we're going to be editing
	const sf::Font& fnt;
	std::map<std::string, bool> &localFlags;
	std::map<std::string, bool> &globalFlags;

	std::vector<sf::Text> requiredTexts;
	std::vector<sf::Text> triggeredTexts;
	std::vector<sf::Text> localTexts;
	std::vector<sf::Text> globalTexts;

	sf::Text breakTexts[4];
	Button* buttons[4];
	const unsigned int charSize = 20;
	int clickedButton;
	std::string inStrings[2];

	sf::RectangleShape rect;

	void moveTextBlock(TextBlocks block, float moveVal);
	void clicked(const sf::Vector2f& mousePos, std::vector<sf::Text>& vec);
	void addText(std::vector<sf::Text>& vec);
};
#endif//FLAGEDITOR_H
