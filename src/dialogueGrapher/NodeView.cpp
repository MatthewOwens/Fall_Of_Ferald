#include "NodeView.h"
#include <iostream>

// Determines the node ID using the current module ID and the number of
// nodes in the module
NodeView::NodeView(const std::string& moduleID, int nodeCount,
				   const sf::Vector2f position, const sf::Font& font)
{
	node = new Node(moduleID + std::to_string(nodeCount));
	this->position = position;

	// Default sizes
	size = sf::Vector2f(100, 100);
	sf::Vector2f iboxSize(size.x - (2 * spacing), size.y / 2 - (2*spacing));
	sf::Vector2f iboxPos = this->position;

	// Initilising the base rectangle
	baseRect = sf::RectangleShape(size);
	baseRect.setPosition(this->position);
	baseRect.setPosition(this->position.x, this->position.y);
	baseRect.setFillColor(sf::Color(100, 32, 32));
	idText = sf::Text(moduleID + std::to_string(nodeCount), font);
	idText.setPosition(this->position);
	idText.move(0, textSpacingY);

	// Creating the header input box
	iboxPos.x += spacing;
	iboxPos.y += spacing;
	headerInput = InputBox(iboxPos, iboxSize, font);

	// Creating the body input box
	iboxPos = headerInput.getPosition();
	iboxPos.y += (iboxSize.y + spacing * 2);
	bodyInput = InputBox(iboxPos, iboxSize, font);
}

void NodeView::setID(const std::string& moduleID, int nodeNumber)
{
	if (node)
	{
		std::string newID = moduleID + std::to_string(nodeNumber);
		node->setID(newID);
		idText.setString(newID);
	}
}

void NodeView::move(const sf::Vector2f& vector)
{
	baseRect.move(vector);
	idText.move(vector);
	headerInput.move(vector);
	bodyInput.move(vector);
}

void NodeView::setScale(float scale)
{
	if(scale < 0.1f)
		return;

	baseRect.setScale(scale, scale);
	const sf::Vector2f& basePos = baseRect.getPosition();
	//sf::Vector2f basePos = sf::Vector2f(baseRect.getGlobalBounds().left, baseRect.getGlobalBounds().top);

	// Repositioning to the base so that we can realign with the spacing*scale later
	idText.setPosition(basePos);
	headerInput.setPosition(basePos);
	bodyInput.setPosition(basePos);

	// Scaling
	//baseRect.setScale(scale, scale);
	idText.setScale(scale, scale);
	headerInput.setScale(scale, scale);
	bodyInput.setScale(scale, scale);

	// Repositioning
	idText.move(0, textSpacingY * scale);
	headerInput.move(spacing*scale, spacing*scale);
	bodyInput.move(spacing*scale, 2*spacing*scale + headerInput.getLocalBounds().height * scale);
}

sf::FloatRect NodeView::getGlobalBounds()
{
	return baseRect.getGlobalBounds();
}

bool NodeView::removeRequired(const sf::Vector2f& mousePos)
{
	if(baseRect.getGlobalBounds().contains(mousePos))
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
InputBox* NodeView::getSelectedInputBox(const sf::Vector2f& mousePos)
{
	if (headerInput.checkClicked(mousePos))
		return &headerInput;
	else if (bodyInput.checkClicked(mousePos))
		return &bodyInput;
	else return NULL;
}

// Returns the header/body value saved in Node if either input box is selected
std::string NodeView::getUnsavedInput()
{
	if (!node)
		return "";

	if (headerInput.isSelected())
		return node->getHeader();
	else if (bodyInput.isSelected())
		return node->getBody();
	else return "";
}

void NodeView::update()
{
	float maxTextWidth;

	// Finding the max text width
	if (headerInput.getString().length() > bodyInput.getString().length())
		maxTextWidth = headerInput.getTextWidth();
	else
		maxTextWidth = bodyInput.getTextWidth();

	// Ensuring that there's always space for one more character
	maxTextWidth += headerInput.getCharacterSize();

	// Do we need to resize?
	if (maxTextWidth < baseRect.getSize().x)
	{
		baseRect.setSize(sf::Vector2f(100 + spacing, baseRect.getSize().y));
		headerInput.setSize(sf::Vector2f(100 - spacing, headerInput.getSize().y));
		bodyInput.setSize(sf::Vector2f(100 - spacing, bodyInput.getSize().y));
		return;
	}

	// Resizing
	baseRect.setSize(sf::Vector2f(maxTextWidth, baseRect.getSize().y));
	headerInput.setSize(sf::Vector2f(maxTextWidth, headerInput.getSize().y));
	bodyInput.setSize(sf::Vector2f(maxTextWidth, bodyInput.getSize().y));
}

void NodeView::updateNodeText()
{
	if (node)
	{
		node->setHeader(headerInput.getString());
		node->setBody(bodyInput.getString());

		// DEBUG
		std::cout << node->getIdentifier() << ":" << std::endl;
		std::cout << "\tHEADER: " << node->getHeader() << std::endl;
		std::cout << "\tBODY: " << node->getBody() << std::endl;
	}
}


void NodeView::render(sf::RenderWindow& window)
{
	window.draw(baseRect);
	window.draw(idText);
	headerInput.render(window);
	bodyInput.render(window);
}

NodeView::~NodeView()
{
	delete node;
	node = 0;
}