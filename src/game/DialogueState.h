#ifndef DIALOGUE_STATE
#define DIALOGUE_STATE
#include "BaseState.h"
#include "InputManager.h"
#include "ImageManager.h"
#include "DialogueBox.h"
#include "Button.h"
#include "../dialogueGrapher/FileManager.h"
#include "../dialogueGrapher/Node.h"
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
	sf::Font font;

	std::vector<Node*> dialogueNodes;
	std::map<std::string, bool> gFlags;
	std::map<std::string, bool> lFlags;
	Node* currentNode;

	std::vector<Button> choiceButtons;

	bool nextNode();
};
#endif//DIALOGUE_STATE