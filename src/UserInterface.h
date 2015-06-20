#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>
#include <list>
#include "Tooltip.h"
#include "DialogueBox.h"
#include "TileHighlight.h"
#include <SFML/System/Vector2.hpp>

class UserInterface
{
    private:
        std::list<Tooltip> tooltips;
        sf::Font regularFont;
        sf::Font italicFont;
        DialogueBox dialogueBox;
		std::vector<TileHighlight> highlights;
    public:
        UserInterface();
        virtual ~UserInterface();
        void addTooltip(std::string header, std::string body, sf::Texture& image, int x, int y);
        void addDialogueBox(std::string scriptPath, sf::Texture& image, int x, int y);
        void update(sf::Vector2f pointerPosition, sf::Vector2f prevPointerPos, bool lmbPressed, bool rmbPressed);
					//const sf::Transform& cameraMatrix);
        void draw(sf::RenderWindow *window, sf::View& camera);
        void nextDialogueLine();
        std::list<Tooltip> getTooltips();
        Tooltip getTooltip(std::string header);
        Tooltip getTooltip(int mouseX, int mouseY);
		const sf::Color friendlyHighlight = sf::Color(0, 255, 255, 123);
		const sf::Color enemyHighlight = sf::Color(255, 0, 0, 123);

		// Highlight functions
		void highlightTiles(std::vector<sf::Vector3i> highlightRange, sf::Color colour, int tileSize);
		void highlightTiles(std::vector<sf::Vector2i> highlightRange, sf::Color colour, int tileSize);
		void highlightTiles(std::stack<sf::Vector2i> highlightRange, sf::Color colour, int tileSize);
		void highlightTile(int x, int y, sf::Color colour, int tileSize);
		inline void clearHighlight() { highlights.clear(); }
		void clearHighlight(sf::Color colour);
};

#endif // USERINTERFACE_H
