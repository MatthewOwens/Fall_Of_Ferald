#include "DialogueState.h"
#include "StateManager.h"

DialogueState::DialogueState() : BaseState()
{
	renderPrevious = true;
	makingChoice = false;
}

DialogueState::~DialogueState()
{

}

void DialogueState::update(InputManager* inputManager, StateManager* stateManager)
{
	bool close = false;
	if (!inputValid)
	{
		std::cout << "Dialogue state could not be entered, input is invalid!" << std::endl;
		stateManager->popState();
	}

	if (inputManager->pressedOnce("cancel"))
		//stateManager->popState();
		close = true;

	if (inputManager->pressedOnce(sf::Mouse::Left))
	{
		if (nextNodeExists())
		{
			if (validConnectionIndices.size() == 1)
			{
				std::vector<Connector>& conns = currentNode->getConnections();
				currentNode = conns[validConnectionIndices[0]].getEnd();

				dialogueBox->setStrings(currentNode->getHeader(), currentNode->getBody());
			}
			else
			{
				makingChoice = true;
				populateButtons();
			}
		}
		else
			//stateManager->popState();
			close = true;
	}

	if (makingChoice)
	{
		// TODO: Check buttons
		makingChoice = false;

		std::vector<Connector>& conns = currentNode->getConnections();
		currentNode = conns[validConnectionIndices[0]].getEnd();
	}

	if (close)
		stateManager->popState();
}

void DialogueState::populateButtons()
{
	// TODO: Actually populate buttons
	std::cout << "TODO: Actually populate buttons" << std::endl;
}

void DialogueState::render(sf::RenderWindow* window)
{
	dialogueBox->draw(window);
}

void DialogueState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	FileManager fm("dialogue/");		// Todo: replace global dialogue folder with one for the current save
	inputValid = false;
	dialogueBox = NULL;

	if (!font.loadFromFile("assets/fonts/steelfish rg.ttf"))
		std::cout << "fonts fucked" << std::endl;

	imageManager->loadImage("assets/images/interface/Dialogue.png", "dBox");

	if (data != NULL)
	{
		if (*data >> dialogueName)
		{
			dialogueNodes = fm.loadDialogue(dialogueName);

			// Ensuring that the dialogue that we loaded actually had nodes
			if (dialogueNodes.size() == 0)
				return;

			lFlags = fm.loadLocals(dialogueName);
			gFlags = fm.loadGlobals();
			inputValid = true;

			dialogueBox = new DialogueBox(imageManager->getTexture("dBox"), font);
			currentNode = dialogueNodes[0];

			dialogueBox->setStrings(currentNode->getHeader(), currentNode->getBody());
		}
	}
}

void DialogueState::onPause()
{

}

sf::Packet DialogueState::onExit(ImageManager* imageManager)
{
	imageManager->unloadImage("dBox");
	for (int i = 0; i < dialogueNodes.size(); ++i)
	{
		delete dialogueNodes[i];
		dialogueNodes[i] = NULL;
	}
	sf::Packet dat;

	return dat;
}

// Returns true if there is another node to switch to
bool DialogueState::nextNodeExists()
{
	std::vector<Connector> conns = currentNode->getConnections();
	int lowestNiceIndex = 0;

	if (conns.size() == 0)	// We're at the end of the dialogue tree
		return false;

	validConnectionIndices.clear();

	// Finding the nodes that we can traverse to
	for (int i = 0; i < conns.size(); ++i)
	{
		const int& connPrio = conns[i].getPriority();
		if (connPrio == conns[lowestNiceIndex].getPriority())
			validConnectionIndices.push_back(i);
		else if (connPrio < conns[lowestNiceIndex].getPriority())
		{
			// New highest priority found
			lowestNiceIndex = i;
			validConnectionIndices.clear();
			validConnectionIndices.push_back(i);
		}
	}


	// Removing connectors who's conditions are not met
	//for (auto i : validConnectionIndices)
	for (auto i = validConnectionIndices.begin(); i != validConnectionIndices.end(); )
	{
		const std::map<std::string, bool>& flags = conns[*i].getFlags();

		// If there's flags to parse
		if (flags.size() >= 1)
		{
			//for (auto flag : flags)
			for (auto flag = flags.begin(); flag != flags.end(); ++flag)
			{
				auto lSearch = lFlags.find(flag->first);
				auto gSearch = gFlags.find(flag->first);

				// Flag can never be triggered, inform the user of the error
				if (lSearch == lFlags.end() && gSearch == gFlags.end())
				{
					std::cout << "Flag value '" << flag->first << "' was not found!";
					i = validConnectionIndices.erase(i);
					break;
				}

				// Prioritising local matches
				if (lSearch != lFlags.end())
				{
					if (lSearch->second != flag->second)	// Flag isn't met
					{
						i = validConnectionIndices.erase(i);
					}
					break;
				}
				else if (gSearch != gFlags.end())
				{
					if (gSearch->second != flag->second) // Flag isn't met
					{
						i = validConnectionIndices.erase(i);
					}
					break;
				}
			}
		}
		i++;
	}

	// There's at least one valid node
	return (validConnectionIndices.size() >= 1);
}
