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
		std::cerr << "Dialogue state could not be entered, input is invalid!" << std::endl;
		stateManager->popState();
	}

	if (inputManager->pressedOnce("cancel"))
		stateManager->pushState(StateManager::PAUSE);
		//stateManager->popState();
		//close = true;

	if (inputManager->pressedOnce(sf::Mouse::Left))
	{
		std::vector<Connector>& conns = currentNode->getConnections();

		if (nextNodeExists())
		{
			if (validConnectionIndices.size() == 1)
			{
				updateFlags(conns[validConnectionIndices[0]]);
				currentNode = conns[validConnectionIndices[0]].getEnd();
				dialogueBox->setStrings(currentNode->getHeader(), currentNode->getBody());
			}
			else
			{
				makingChoice = true;

				if (choiceButtons.size() == 0)
					populateButtons();
			}
		}
		else
			//stateManager->popState();
			close = true;


		if (makingChoice)
		{
			for (int i = 0; i < choiceButtons.size(); ++i)
			{
				choiceButtons[i]->update(inputManager);
				if (choiceButtons[i]->isPressed())
				{
					makingChoice = false;
					updateFlags(conns[validConnectionIndices[i]]);
					currentNode = conns[validConnectionIndices[i]].getEnd();
					dialogueBox->setStrings(currentNode->getHeader(), currentNode->getBody());
					break;
				}
			}

			// If a selection was made
			if (!makingChoice)
			{
				for (auto btn : choiceButtons)
					delete btn;

				choiceButtons.clear();
			}
		}
	}

	if (close)
		stateManager->popState();
}

void DialogueState::populateButtons()
{
	sf::Vector2f startPos(500, 200);	// TODO: base on screen size
	sf::Vector2f size(200, 100);
	std::vector<Connector> conns = currentNode->getConnections();
	
	for (auto i : validConnectionIndices)
	{
		choiceButtons.push_back(new Button(size, sf::Color(40, 40, 170)));
		choiceButtons.back()->setText(conns[i].getChoiceText(), font);
		choiceButtons.back()->setPosition(startPos);
		startPos.y += size.y;
	}
}

void DialogueState::render(sf::RenderWindow* window)
{
	dialogueBox->draw(window);

	for (auto i : choiceButtons)
		i->draw(window);
}

void DialogueState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	FileManager fm("dialogue/");		// Todo: replace global dialogue folder with one for the current save
	inputValid = false;
	dialogueBox = NULL;

	if (!font.loadFromFile("assets/fonts/steelfish rg.ttf"))
		std::cerr << "fonts fucked" << std::endl;

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
	FileManager fm("dialogue/");
	fm.saveLocals(dialogueName, lFlags);
	fm.saveGlobals(gFlags);

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

	if (conns.size() == 0)	// We're at the end of the dialogue tree
		return false;

	validConnectionIndices.clear();

	for (int i = 0; i < conns.size(); ++i)
		validConnectionIndices.push_back(i);

	// Removing connectors who's conditions are not met
	for (auto i = validConnectionIndices.begin(); i != validConnectionIndices.end(); )
	{
		const std::map<std::string, bool>& flags = conns[*i].getFlags();
		bool incRequired = true;

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
					std::cerr << "Flag value '" << flag->first << "' was not found!";
					i = validConnectionIndices.erase(i);
					incRequired = false;
					break;
				}

				// Prioritising local matches
				if (lSearch != lFlags.end())
				{
					if (lSearch->second != flag->second)	// Flag isn't met
					{
						i = validConnectionIndices.erase(i);
						incRequired = false;
					}
					break;
				}
				else if (gSearch != gFlags.end())
				{
					if (gSearch->second != flag->second) // Flag isn't met
					{
						i = validConnectionIndices.erase(i);
						incRequired = false;
					}
					break;
				}
			}
		}
		if (incRequired)
			i++;
	}

	// Finding the highest valid priority
	int lowestNiceIndex = validConnectionIndices[0];

	for (auto i : validConnectionIndices)
	{
		if (conns[i].getPriority() < conns[lowestNiceIndex].getPriority())
			lowestNiceIndex = i;
	}

	// Invalidating lower priority connections
	for (auto itr = validConnectionIndices.begin(); itr != validConnectionIndices.end();)
	{
		if (conns[*itr].getPriority() > conns[lowestNiceIndex].getPriority())
			itr = validConnectionIndices.erase(itr);
		else ++itr;
	}

	// There's at least one valid node
	return (validConnectionIndices.size() >= 1);
}


void DialogueState::updateFlags(Connector& selectedConn)
{
	for (auto flag : selectedConn.getTriggers())
	{
		// Searching local flags
		auto lSearch = lFlags.find(flag.first);
		if (lSearch != lFlags.end())
		{
			lSearch->second = flag.second;
		}
		else
		{
			// Searching global flags
			auto gSearch = gFlags.find(flag.first);
			if (gSearch != gFlags.end())
				gSearch->second = flag.second;
		}

	}
}
