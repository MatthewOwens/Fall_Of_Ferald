#ifndef DIALOGUE_STATE
#define DIALOGUE_STATE
#include "BaseState.h"
#include "InputManager.h"
#include "ImageManager.h"
#include "DialogueBox.h"
//#include "../dialogueGrapher/FileManager.h"

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
};
#endif//DIALOGUE_STATE