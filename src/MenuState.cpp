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
	//imageManager->loadImage("assets/images/interface/buttons/tempButtonIcon.png", "buttonIcon");
	imageManager->loadImage("assets/images/interface/buttons/menuButton.png", "buttonIcon");

	buttonFont.loadFromFile("assets/fonts/steelfish rg.ttf");

	menubgSprite = new sf::Sprite(imageManager->getTexture("menubg"));
	menubgSprite->scale(0.75f, 0.75f);

	button = new Button(imageManager->getTexture("buttonIcon"));
	button->move(sf::Vector2f(1280.f/2 - 250, 720.f/2 - 200));
	button->getBaseTransformable()->scale(0.25f,0.25f);
	button->setText(sf::String("Start"), buttonFont, 42);

	exitButton = new Button(imageManager->getTexture("buttonIcon"));
	exitButton->move(sf::Vector2f(1280.f/2 - 250, 720.f/2 + 50));
	exitButton->getBaseTransformable()->scale(0.25f,0.25f);
	exitButton->setText(sf::String("Exit"), buttonFont, 42);

	// Making the start button juicy
	sf::Vector2f startPos = button->getPosition();
	startPos.x -= 100;
	juicer.add (button->getBaseTransformable(), Juicer::JuiceType::ELASTIC_OUT,
				Juicer::TRS::TRANSLATE, sf::milliseconds(1000), startPos, button->getPosition());

	startPos = button->getTextTransformable()->getPosition();
	startPos.x -= 100;
	juicer.add (button->getTextTransformable(), Juicer::JuiceType::ELASTIC_OUT,
				Juicer::TRS::TRANSLATE, sf::milliseconds(1000), startPos,
				button->getTextTransformable()->getPosition());

	// Making the end button juicy
	startPos = exitButton->getPosition();
	startPos.x += 100;
	juicer.add (exitButton->getBaseTransformable(), Juicer::JuiceType::ELASTIC_OUT,
				Juicer::TRS::TRANSLATE, sf::milliseconds(1000), startPos, exitButton->getPosition());

	startPos = exitButton->getTextTransformable()->getPosition();
	startPos.x += 100;
	juicer.add (exitButton->getTextTransformable(), Juicer::JuiceType::ELASTIC_OUT,
				Juicer::TRS::TRANSLATE, sf::milliseconds(1000), startPos,
				exitButton->getTextTransformable()->getPosition());

	//juicer.add (button->getBaseTransformable(), Juicer::JuiceType::LERP, Juicer::TRS::ROTATE, sf::milliseconds(500), 0, 1);
	//juicer.add (button->getBaseTransformable(), Juicer::JuiceType::LERP, Juicer::TRS::SCALE, sf::milliseconds(500), 0, 1);
}

sf::Packet MenuState::onExit(ImageManager* imageManager)
{
	imageManager->unloadImage("menubg");
	imageManager->unloadImage("buttonIcon");

	delete menubgSprite;
	menubgSprite = NULL;

	delete button;
	button = NULL;

	delete exitButton;
	exitButton = NULL;

	return bundle;
}

void MenuState::onPause()
{
}

void MenuState::update(InputManager* inputManager, StateManager* stateManager)
{
	button->update(inputManager);
	exitButton->update(inputManager);
	juicer.update();

	//if(inputManager->pressedOnce("confirm"))
	if(button->isPressed())
	{
		stateManager->pushState(StateManager::GAME);
	}

	//if(inputManager->pressedOnce("cancel"))
	if(exitButton->isPressed() || inputManager->pressedOnce("cancel"))
		stateManager->popState();
}

void MenuState::render(sf::RenderWindow* window)
{
	if(window != NULL)
	{
		window->draw(*menubgSprite);
		button->draw(window);
		exitButton->draw(window);
	}
}
