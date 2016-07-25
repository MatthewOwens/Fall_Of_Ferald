#include "DialogueState.h"
#include "StateManager.h"

DialogueState::DialogueState() : BaseState()
{
	renderPrevious = true;
}

DialogueState::~DialogueState()
{

}

void DialogueState::update(InputManager* inputManager, StateManager* stateManager)
{
	if (!inputValid)
	{
		std::cout << "Dialogue state could not be entered, input is invalid!" << std::endl;
		stateManager->popState();
	}
	else
	{
		std::cout << "Dialogue input valid!" << std::endl;
		stateManager->popState();
	}
}

void DialogueState::render(sf::RenderWindow* window)
{

}

void DialogueState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	FileManager fm("dialogue/");		// Todo: replace global dialogue folder with one for the current save

	inputValid = false;

	if (data != NULL)
	{
		if (*data >> dialogueName)
		{
			dialogueNodes = fm.loadDialogue(dialogueName);
			lFlags = fm.loadLocals(dialogueName);
			gFlags = fm.loadGlobals();
			inputValid = true;
		}
	}
}

void DialogueState::onPause()
{

}

sf::Packet DialogueState::onExit(ImageManager* imageManager)
{
	return sf::Packet();
}

//sf::Packet& operator << (sf::Packet& packet, s)