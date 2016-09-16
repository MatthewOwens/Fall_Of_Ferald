#ifndef COMBATANIMSTATE_H
#define COMBATANIMSTATE_H
#include "BaseState.h"
#include "InputManager.h"
#include "Shader.h"
#include <SFML/OpenGL.hpp>

class StateManager;

class CombatAnimState : public BaseState
{
public:
	CombatAnimState();
	~CombatAnimState();
	void update(InputManager* inputManager, StateManager* stateManager);
	void render(sf::RenderWindow* window);
	void onEnter(sf::Packet* data, ImageManager* imageManager);
	void onPause();
	sf::Packet onExit(ImageManager* imageManager);
private:
	sf::Packet data;
	sf::RectangleShape rect;

	GLuint VBO;
	GLuint VAO;
	Shader* shader = NULL;

	GLfloat vertices[9] = {
		-0.5f, -0.5f, 0.f,
		0.5f, -0.5f, 0.f,
		0.f, 0.5f, 0.f };
};
#endif//COMBATANIMSTATE_H
