// The level class is responsible for managing each individual level.
// This includes Units and Tiles

#ifndef LEVEL_H
#define LEVEL_H
#include <string>
#include <fstream>
#include "Tile.h"
#include "ImageManager.h"
#include "InputManager.h"
#include "Pathfinder.h"
#include "GameUserInterface.h"
#include "AI.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>


class Level
{
    public:
        Level(const std::string& mapPath, const std::string& tilesheetPath, ImageManager* imageManager);
        Level(const std::string& mapPath);
        void initilizeAI(const std::string& unitPath, const std::string& spritesheetPath, ImageManager& imageManager);
        void draw(sf::RenderWindow& window);
        void update(InputManager& inputManager, GameUserInterface& ui);
        virtual ~Level();
        Tile getTile(int x, int y);
        std::string getTileType(int x, int y);
        int getMapSizeX();
        int getMapSizeY();

        Tile** getTiles() { return tiles; }

        // Accessor methods
        inline int getTileSize() { return tileSize; }
        inline AI& getAI() { return combatController; }
        void nextTurn();
        inline bool isPlayerTurn() { return playerTurn; }

    private:
        enum TurnState {SELECTION, MOVE, MOVEANIM, ATTACK, ATTACKANIM};
        TurnState turnState;
        sf::Clock frameClock;


        int levelWidth;
        int levelHeight;
        const int tileSize = 64;
        bool playerTurn;
        bool playerUnitSelected;
        bool playerUnitTargeting;
        sf::Vector2i hoveredTile;			// The tile the mouse is currently hovering over
        sf::Vector2i previouslyHoveredTile;
        //std::vector<sf::Vector3i> toHighlight;
        std::vector<sf::Vector3i> toHighlight;
        std::vector<sf::Vector2i> toHighlightAtk;
        std::stack<sf::Vector2i> pathStack;
        sf::Vector2i selectedUnitPos;
        Unit* selectedUnit;
        Tile** tiles;
        AI combatController;
        Pathfinder pathfinder;

        void deselectUnit(GameUserInterface& ui, bool moveBack = true);
};

#endif // LEVEL_H
