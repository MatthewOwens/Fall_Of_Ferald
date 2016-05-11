#ifndef INPUTBOX_H
#define INPUTBOX_H
//#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
class InputBox
{
	public:
		InputBox(){}
		InputBox(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font);
		~InputBox();
		void addCharacter(const char& character);
		void addCharacter(const std::string& character);
		void removeCharacter();

		void render(sf::RenderWindow& window);
		void checkClicked(const sf::Vector2f& mousePos);
		inline bool isSelected() { return selected; }
		inline std::string getString() { return text.getString(); }
	private:
		sf::Text text;
		sf::RectangleShape baseBox;
		bool selected;
};
#endif//INPUTBOX_H
