#ifndef INPUTBOX_H
#define INPUTBOX_H
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
class InputBox : public sf::RectangleShape
{
	public:
		InputBox(){}
		InputBox(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, bool scalable = true);
		~InputBox();
		void addCharacter(const char& character);
		void addCharacter(const std::string& character);
		void removeCharacter();

		void render(sf::RenderWindow& window);
		bool checkClicked(const sf::Vector2f& mousePos);
		void setSelected(bool val); 
		inline bool isSelected() { return selected; }
		inline void setActive(bool active) { this->active = active; }
		inline std::string getString() { return text.getString(); }
		inline void clear() { text.setString("");}
		inline void setString(std::string str) { text.setString(str); }

		// Overriding sf::RectangleShape's functions
		void move(const sf::Vector2f& offset);
		void move(float offsetX, float offsetY);

		void setScale(const sf::Vector2f& factor);
		void setScale(float factorX, float factorY);
	private:
		sf::Text text;
		bool selected;
		bool active;
		bool scalable;
};
#endif//INPUTBOX_H
