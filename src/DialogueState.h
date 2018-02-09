#ifndef DIALOGUE_STATE
#define DIALOGUE_STATE
#include "BaseState.h"
#include "InputManager.h"
#include "ImageManager.h"
#include "DialogueBox.h"
#include "Button.h"
#include "FileManager.h"
#include "Node.h"
#include "Connector.h"
#include <vector>
#include <string>
#include <map>

class StateManager;

class DialogueState : public BaseState
{
public:
	DialogueState();
	~DialogueState();
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	sf::Packet onExit(ImageManager* imageManager);
private:
	bool inputValid;
	std::string dialogueName;

	DialogueBox* dialogueBox;
	sf::Texture* buttonTexture;
	sf::Font font;

	sf::Color deselectColor;
	sf::Sprite playerSprite;
	sf::Sprite partnerSprite;
	const std::string charSpriteFolder;
	std::string partnerData;

	std::vector<Node*> dialogueNodes;
	std::map<std::string, int> gFlags;
	std::map<std::string, int> lFlags;
	Node* currentNode;
	bool makingChoice;

	std::vector<Button*> choiceButtons;
	std::vector<int> validConnectionIndices;

	bool nextNodeExists();
	void populateButtons();
	void updateFlags(Connector& selectedConn);
};
#endif//DIALOGUE_STATE
