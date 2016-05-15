#include "NodeView.h"

// Determines the node ID using the current module ID and the number of
// nodes in the module
NodeView::NodeView(const std::string& moduleID, int nodeCount,
				   const sf::Vector2f position, const sf::Font& font)
{
	node = new Node(moduleID + std::to_string(nodeCount));
	this->position = position;

	// Default sizes
	size = sf::Vector2f(100, 100);
	sf::Vector2f textboxSize(size.x - (2 * spacing), size.y / 2 - (2*spacing));

	for(int i = 0; i < 3; ++i)
	{
		rects[i] = sf::RectangleShape(position);
		rects[i].setPosition(this->position);

		texts[i] = sf::Text("", font);
		texts[i].setPosition(this->position);

		// Base box & text
		if(i == 0)
		{
			rects[i].setSize(size);
			rects[i].setFillColor(sf::Color(100,32,32));
			texts[i].setString(node->getIdentifier());
			texts[i].setPosition(this->position.x, this->position.y - 35);
		}
		else
		{
			rects[i].setPosition(this->position.x + spacing,
								 (this->position.y * i) + spacing);
			rects[i].setSize(textboxSize);
		}
	}
}

void NodeView::move(const sf::Vector2f& vector)
{
	for(int i = 0; i < 3; ++i)
	{
		rects[i].move(vector);
		texts[i].move(vector);
	}
}

void NodeView::setScale(float scale)
{
	if(scale < 0)
		return;

	for(int i = 0; i < 3; ++i)
	{
		rects[i].setScale(scale, scale);
		texts[i].setScale(scale, scale);
	}
}

sf::FloatRect NodeView::getGlobalBounds()
{
	return rects[0].getGlobalBounds();
}

bool NodeView::removeRequired(const sf::Vector2f& mousePos)
{
	if(rects[0].getGlobalBounds().contains(mousePos))
	{
		delete node;
		node = NULL;
		return true;
	}
	else
	{
		return false;
	}
}


void NodeView::render(sf::RenderWindow& window)
{
	for(int i = 0; i < 3; ++i)
	{
		window.draw(rects[i]);
		window.draw(texts[i]);
	}
}

NodeView::~NodeView()
{
	delete node;
	node = 0;
}

