#include "CombatAnimState.h"
#include <SFML/OpenGL.hpp>
//#include <GL/gl.h>
#include <iostream>
#include "StateManager.h"


CombatAnimState::CombatAnimState()
{
}

CombatAnimState::~CombatAnimState()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	delete shader;
}

void CombatAnimState::update(InputManager* inputManager, StateManager* stateManager)
{
	if(inputManager->pressedOnce("cancel"))
		stateManager->popState();
}

void CombatAnimState::render(sf::RenderWindow* window)
{
	window->pushGLStates();

	glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Drawing the triangle
	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);	// Unbind VAO
	glDisableVertexAttribArray(0);

	window->popGLStates();
	window->resetGLStates();
	window->draw(rect);
}

void CombatAnimState::onEnter(sf::Packet* data, ImageManager* imageManager)
{
	renderPrevious = true;

	// Setting up the rect
	rect.setSize(sf::Vector2f(50.f, 50.f));
	rect.setPosition(0,0);
	rect.setFillColor(sf::Color::White);

	shader = new Shader("shaders/triangle.vs", "shaders/triangle.fs");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Colour attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);	// Unbind VAO
}

void CombatAnimState::onPause()
{
}

sf::Packet CombatAnimState::onExit(ImageManager* imageManager)
{
	return data;
}
