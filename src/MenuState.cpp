#include "MenuState.h"
#include "StateManager.h"

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	imageManager->loadImage("assets/images/interface/menubg.png", "menubg");
	imageManager->loadImage("assets/images/interface/buttons/tempButton.png", "button");
	imageManager->loadImage("assets/images/interface/buttons/tempButtonIcon.png", "buttonIcon");

	buttonFont.loadFromFile("assets/fonts/steelfish rg.ttf");

	menubgSprite = new sf::Sprite(imageManager->getTexture("menubg"));
	menubgSprite->scale(0.75f, 0.75f);

	button = new Button(imageManager->getTexture("button"));
	button->setText(sf::String("text"), buttonFont);
	button->setIcon(imageManager->getTexture("buttonIcon"));
	//button->move(sf::Vector2f(50.f, 10.f));
}

sf::Packet MenuState::onExit(ImageManager* imageManager)
{
	imageManager->unloadImage("menubg");
	imageManager->unloadImage("button");
	imageManager->unloadImage("buttonIcon");

	delete menubgSprite;
	menubgSprite = NULL;

	delete button;
	button = NULL;

	return bundle;
}

void MenuState::onPause()
{
}

void MenuState::update(InputManager* inputManager, StateManager* stateManager)
{
	//if(inputManager->pressedOnce("confirm"))
	if(button->isPressed(inputManager))
	{
		std::cout << "Changing to game state" << std::endl;
		stateManager->pushState(StateManager::GAME);
	}

	if(inputManager->pressedOnce("cancel"))
		stateManager->popState();
}

void MenuState::render(sf::RenderWindow* window)
{
	if(window != NULL)
	{
		window->clear();
		window->draw(*menubgSprite);
		button->draw(window);
		window->display();
	}
}
