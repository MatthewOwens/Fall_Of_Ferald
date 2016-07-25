#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>


class DialogueBox
{
    public:
		DialogueBox(const sf::Texture& texture, const sf::Font font,
					const int charSize = 48, const int posX = 0, const int posY = 0);

        virtual ~DialogueBox();
        void draw(sf::RenderWindow *window);
		void setStrings(const std::string& header, const std::string& body);

    private:
        sf::Text name;          // The name of the character that is currently talking
        sf::Text dialogue;      // The dialogue being displayed
        sf::Sprite sprite;
};

#endif // DIALOGUEBOX_H
