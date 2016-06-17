/// Class to display nodes and connectors and provide an interface to edit
/// them.
#ifndef NODEVIEW_H
#define NODEVIEW_H
#include "Node.h"
#include "InputBox.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
class NodeView
{
	public:
		NodeView(const std::string& moduleID, int nodeCount,
				 const sf::Vector2f position, const sf::Font& font);
		~NodeView();
		NodeView(){}
		void render(sf::RenderWindow& window);
		void setScale(float scale);
		void setID(const std::string& moduleID, int nodeNumber);
		bool removeRequired(const sf::Vector2f& mousePos);
		void move(const sf::Vector2f& vector);
		sf::FloatRect getGlobalBounds();
	private:
		// Enum for accessing the text and rect arrays
		enum {ID, HEADER, BODY};

		Node* node = NULL;
		sf::RectangleShape baseRect;
		sf::Text idText;
		InputBox headerInput;
		InputBox bodyInput;

		/*sf::RectangleShape rects[3];
		sf::Text texts[3];*/

		sf::Vector2f position;
		sf::Vector2f size;
		const float spacing = 5.f;
};
#endif//NODEVIEW_H
