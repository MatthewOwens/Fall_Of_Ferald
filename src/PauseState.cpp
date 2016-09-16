#include "PauseState.h"
#include "StateManager.h"

PauseState::PauseState() : BaseState()
{
}

PauseState::~PauseState()
{
}

void PauseState::update(InputManager* inputManager, StateManager* stateManager)
{
	for(auto btn : buttons)
		btn.second->update(inputManager);

	if(inputManager->pressedOnce("cancel") || buttons["continue"]->isPressed())
		stateManager->popState();

	if(buttons["exit"]->isPressed())
		stateManager->popTo(StateManager::MENU);
}

void PauseState::render(sf::RenderWindow* window)
{
	// Drawing the overlay
	window->draw(background);

	for(auto btn : buttons)
		btn.second->draw(window);
}

/**
  Initilises the Pause State
  @Param[in]	data Contains screen bounds x, y
  @Param[in]	imageManager pointer to the ImageManager
 */
void PauseState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	renderPrevious = true;

	if(data != NULL)
	{
		*data >> windowBounds.x;
		*data >> windowBounds.y;
		background.setSize((sf::Vector2f)windowBounds);
		background.setFillColor(sf::Color(43,43,43,150));

		sf::Vector2f btnSize(100, 200);
		float offset = 5.f;

		// Initilizing the buttons
		imageManager->loadImage("assets/images/interface/buttons/tempButtonIcon.png", "btnIcon");
		buttonFont.loadFromFile("assets/fonts/steelfish rg.ttf");

		buttons["continue"] = new Button(imageManager->getTexture("btnIcon"));
		buttons["exit"] = new Button(imageManager->getTexture("btnIcon"));

		int count = 0;
		for(auto btn : buttons)
		{
			btn.second->setText(sf::String(btn.first), buttonFont);
			btn.second->move(sf::Vector2f(windowBounds.x / 2 - btnSize.x / 2,
										  windowBounds.y / 4 + count * (btnSize.y + offset)));
			count++;
		}
	}
}

void PauseState::onPause()
{
}

sf::Packet PauseState::onExit(ImageManager* imageManager)
{
	imageManager->unloadImage("btnIcon");
	return bundle;
}
