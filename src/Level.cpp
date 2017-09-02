#include "Level.h"
#include <sstream>
#include <iostream>
#include "Unit.h"
#include <cmath>
//#include "SaveManager.h"

Level::Level(const std::string& mapPath)
{
    //SaveManager saveManager;
    std::ifstream inFile(mapPath);
    std::istringstream subString;
    std::string line;
    levelWidth = 0;
    levelHeight = 0;
    playerTurn = true;
    turnState = TurnState::SELECTION;
    playerUnitSelected = false;
    playerUnitTargeting = false;
    hoveredTile = sf::Vector2i(0,0);
    selectedUnitPos = sf::Vector2i(-1,-1);
    previouslyHoveredTile = sf::Vector2i(-1,-1);
    selectedUnit = NULL;
    combatants[0] = NULL;
    combatants[1] = NULL;

    if(inFile.good())
    {
        int i = 0;      // Iterator for within the lines of text
        int j  = 0;		// Iterator for the lines themselves
        bool calculatingWidth = true;

        // Calculating the level size
        while (std::getline(inFile, line))
        {
            if (calculatingWidth)
            {
                subString.str(line);
                levelWidth = line.length();
                calculatingWidth = false;
            }

            levelHeight++;
        }

        // Returning the cursor
        inFile.clear();
        inFile.seekg(0, inFile.beg);

        // Defining the array
        tiles = new Tile*[levelWidth];
        for(int k = 0; k < levelWidth; ++k)
            tiles[k] = new Tile[levelHeight];

        while (std::getline(inFile, line))
        {
            if (j < levelHeight)
            {
                char id;
                subString.str(line);

                while (i < levelWidth)
                {
                    // Grabbing a character from the line string
                    id = subString.get();

                    int identifier = id - '0';
                    tiles[i][j] = Tile(identifier);
                    tiles[i][j].sprite.setPosition(i * tileSize, j * tileSize);
                    i++;
                }
                i = 0;  // Line parsed, moving back to the start
            }
            // Line parsed, move to the next line
            j++;

            // Clearing up the subString
            subString.str("");
            subString.clear();
        }
    }
    else
    {
        std::cerr << "Error loading " << mapPath << ", file not found." << std::endl;
    }

}

Level::Level(const std::string& mapPath, const std::string& tileSheetPath, ImageManager* imageManager)
{
    //SaveManager saveManager;
    std::ifstream inFile(mapPath);
    std::istringstream subString;
    std::string line;
    imageManager->loadImage(tileSheetPath);
    levelWidth = 0;
    levelHeight = 0;
    playerTurn = true;
    playerUnitSelected = false;
    playerUnitTargeting = false;
    hoveredTile = sf::Vector2i(0,0);
    selectedUnitPos = sf::Vector2i(-1,-1);
    previouslyHoveredTile = sf::Vector2i(-1,-1);
    selectedUnit = NULL;

    if(inFile.good())
    {
        int i = 0;      // Iterator for within the lines of text
        int j  = 0;		// Iterator for the lines themselves
        bool calculatingWidth = true;

        // Calculating the level size
        while (std::getline(inFile, line))
        {
            if (calculatingWidth)
            {
                subString.str(line);
                levelWidth = line.length();
                calculatingWidth = false;
            }

            levelHeight++;
        }

        // Returning the cursor
        inFile.clear();
        inFile.seekg(0, inFile.beg);

        // Defining the array
        tiles = new Tile*[levelWidth];
        for(int k = 0; k < levelWidth; ++k)
            tiles[k] = new Tile[levelHeight];

        while (std::getline(inFile, line))
        {
            if (j < levelHeight)
            {
                char id;
                subString.str(line);

                while (i < levelWidth)
                {
                    // Grabbing a character from the line string
                    id = subString.get();

                    int identifier = id - '0';
                    tiles[i][j] = Tile(identifier, imageManager->getTexture(tileSheetPath));
                    tiles[i][j].sprite.setPosition(i * tileSize, j * tileSize);
                    i++;
                }
                i = 0;  // Line parsed, moving back to the start
            }
            // Line parsed, move to the next line
            j++;

            // Clearing up the subString
            subString.str("");
            subString.clear();
        }
    }
    else
    {
        std::cerr << "Error loading " << mapPath << ", file not found." << std::endl;
    }
}

// Method to initilize the AI. Seperate from the constructor on the off chance that there is a level
// with no enemies or NPC allies.
void Level::initilizeAI(const std::string& unitPath, const std::string& spritesheetPath, ImageManager& imageManager)
{
    combatController = AI(unitPath, "stats/");

    // Loading the images for the NPC units
    imageManager.loadAnimation(spritesheetPath + "/mage.png", "mage", 64, 64, 1);
    imageManager.loadAnimation(spritesheetPath + "/warrior.png", "warrior", 64, 64, 1);
    imageManager.loadAnimation(spritesheetPath + "/tank.png", "tank", 64, 64, 1);

    imageManager.loadAnimation(spritesheetPath + "/slime_full.png", "player", 64, 64, 5);

	// Setting the sprites for the NPC units, works as long as the generic unit
	// names are the same as the imageManager keys
	for(auto &unit : combatController.getAvailableUnits())
            unit.setAnimation(imageManager.getAnimation(unit.getName()));

	// Adding a unit for the AI to fight, normally these would be loaded from a file but eh.
	combatController.addEnemyUnit(Unit("", "player", 5, 9999, 10, 0, 6, 3, 16, 2, 3, 3, 1, 3));
	combatController.getEnemyUnits().back().setAnimation(imageManager.getAnimation("player"));
	combatController.getEnemyUnits().back().getSprite().setFrameTime(sf::seconds(0.15));

	combatController.addEnemyUnit(Unit("test", "tank", 5, 9999, 10, 0, 6, 3, 16, 2, 3, 3, 8, 3));
	combatController.getEnemyUnits().back().setAnimation(imageManager.getAnimation("player"));
	combatController.getEnemyUnits().back().getSprite().setFrameTime(sf::seconds(0.15));

	combatController.updateSprites(tileSize);

	// Initilising the pathfinder
	pathfinder = Pathfinder(this);
}

void Level::update(InputManager& inputManager, StateManager& stateManager, GameUserInterface& ui)
{
	// Updating the sprites
	sf::Vector2f mousePos = inputManager.getMousePosition();
	sf::Time frameTime = frameClock.restart();

	if(!playerTurn)
	{
		combatController.updateSprites(tileSize);
		combatController.update(pathfinder, tiles, tileSize);
		nextTurn();
	}
	else
	{
		// Checking if the AI's turn has been initiated
		if(inputManager.pressedOnce("nextTurn"))
			nextTurn();

		// Finding the tile that the cursor is hovered over
		for(int y = 0; y < levelHeight; ++y)
		{
			for(int x = 0; x < levelWidth; ++x)
			{
				if(tiles[x][y].getSprite().getGlobalBounds().contains(mousePos))
				{
					hoveredTile = sf::Vector2i(x,y);
					break;
				}
			}
		}

		switch (turnState)
		{
			case SELECTION:
			{
				// Rendering unit stats on right click
				if(inputManager.pressedOnce(sf::Mouse::Button::Right))
				{
					// Checking player units
					for(auto &unit : combatController.getEnemyUnits())
					{
						if(hoveredTile.x == unit.getX() && hoveredTile.y == unit.getY())
						{
							ui.addTooltip(unit.getName(), unit.getInfo(),
									(hoveredTile.x + 1) * tileSize, hoveredTile.y * tileSize, 24);
						}
					}

					// Checking the AI units
					for(auto &unit : combatController.getAvailableUnits())
					{
						if(hoveredTile.x == unit.getX() && hoveredTile.y == unit.getY())
						{
							ui.addTooltip(unit.getName(), unit.getInfo(),
									(hoveredTile.x + 1) * tileSize, hoveredTile.y * tileSize, 24);
						}
					}

					ui.clearHighlight();
					toHighlight.clear();
					toHighlightAtk.clear();
				}

				// Making a selection on left click
				if(inputManager.pressedOnce(sf::Mouse::Button::Left))
				{
					ui.clearHighlight();
					toHighlight.clear();
					toHighlightAtk.clear();

					playerUnitSelected = false;

					// Checking the player-controlled units to see if we've selected one
					for(auto &unit : combatController.getEnemyUnits())
					{
						if(hoveredTile.x == unit.getX() && hoveredTile.y == unit.getY()
							&& !unit.getMoved())
						{

							pathfinder.calculateArea(unit, toHighlight, toHighlightAtk, true);

							ui.highlightTiles(toHighlight, ui.friendlyHighlight, tileSize);
							ui.highlightTiles(toHighlightAtk, ui.enemyHighlight, tileSize);
							playerUnitSelected = true;
							selectedUnitPos = sf::Vector2i(unit.getX(), unit.getY());
							selectedUnit = &unit;

							// Switching to the next state
							turnState = TurnState::MOVE;

							// No need to check the rest of the units
							break;
						}
					}

					for(auto &unit : combatController.getAvailableUnits())
					{
						if(hoveredTile.x == unit.getX() && hoveredTile.y == unit.getY())
						{

							pathfinder.calculateArea(unit, toHighlight, toHighlightAtk);

							ui.highlightTiles(toHighlight, ui.friendlyHighlight, tileSize);
							ui.highlightTiles(toHighlightAtk, ui.enemyHighlight, tileSize);
							break;
						}
					}
				}
				break;
			}
			case MOVE:
			{
				bool validTile = false;

				// Clearing selection on right click
				if(inputManager.pressedOnce(sf::Mouse::Button::Right))
				{
					deselectUnit(ui);
					break;
				}

				// Drawing the path between the selected unit and the mouse
				if(playerUnitSelected && hoveredTile != previouslyHoveredTile && !playerUnitTargeting)
				{
					// Checking if the hovered tile in our range
					for(auto &i : toHighlight)
					{
						if(i.x == hoveredTile.x && i.y == hoveredTile.y)
						{
							bool occupied = false;
							for(auto otherUnit : combatController.getEnemyUnits())
							{
								if(i.x == otherUnit.getX() && i.y == otherUnit.getY())
									occupied = true;
							}

							if(!occupied)
								validTile = true;
							break;
						}
					}

					if(validTile)
					{
						pathStack = pathfinder.getPath(toHighlight, selectedUnitPos, hoveredTile);

						ui.clearHighlight(ui.enemyHighlight);
						ui.highlightTiles(pathStack, ui.enemyHighlight, tileSize);
					}
					else	// Moved off the movable tiles
					{
						ui.clearHighlight();

						ui.highlightTiles(toHighlight, ui.friendlyHighlight, tileSize);
						ui.highlightTiles(toHighlightAtk, ui.enemyHighlight, tileSize);

						// Clearing the path stack
						while(!pathStack.empty())
							pathStack.pop();
					}
				}

				if(inputManager.pressedOnce(sf::Mouse::Button::Left))
				{
					// If we've clicked in a valid tile for movement
					if(validTile || hoveredTile == selectedUnitPos)
					{
						ui.clearHighlight();
						turnState = TurnState::MOVEANIM;
					}
				}

				break;
			}
			case MOVEANIM:
			{
				//turnState = TurnState::ATTACK;

				if(!pathStack.empty())
					selectedUnit->moveAlong(pathStack, tileSize);	// TODO: Fix movement

				if(pathStack.empty())
					turnState = TurnState::ATTACK;
				break;
			}
			case ATTACK:
			{
				// Highlighting the attackable tiles
				ui.clearHighlight();
				std::vector<sf::Vector2i> atkRange = combatController.getItemRange(*selectedUnit, 5, 2);
				ui.highlightTiles(atkRange, ui.enemyHighlight, tileSize);

				// Clearing selection on right click
				if(inputManager.pressedOnce(sf::Mouse::Button::Right))
				{
					deselectUnit(ui);
					break;
				}

				if(inputManager.pressedOnce(sf::Mouse::Button::Left))
				{
					// Wait if the unit was clicked again
					if(hoveredTile == selectedUnit->getGridPos())
					{
						turnState = TurnState::ATTACKANIM;
					}
					else
					{
						// Checking to see if we're attacking an enemy
						auto unit = combatController.getAvailableUnits().begin();
						while(unit != combatController.getAvailableUnits().end())
						//for(auto &unit : combatController.getAvailableUnits())
						{
							if(unit->getGridPos() == (sf::Vector2i)hoveredTile)
							{
								// TODO: Find out why this valididty check is breaking everything
								bool valid = false;
								for(auto i : atkRange)
								{
									if(i == unit->getGridPos())
										valid = true;
								}

								if(valid)
								{
            								combatants[0] = selectedUnit;
            								combatants[1] = &*unit;
									unit->modifyStat("health", 10);	// TODO: replace temp value (10)
									turnState = TurnState::ATTACKANIM;

									if(unit->getStat("health") <= 0)
										unit = combatController.getAvailableUnits().erase(unit);
									else ++unit;

									break;
								}
								else ++unit;
							}
							else ++unit;
						}
					}
				}

				break;
			}
			case ATTACKANIM:
			{
        			if(combatants[0] != NULL && combatants[1] != NULL)
            			{
                			std::cout << "HERE WE GO LADS" << std::endl;
                			sf::Packet data;
                			data << combatants[0]->getName();
                			data << getTile(combatants[0]->getX(), combatants[0]->getY()).getType();
                			data << combatants[1]->getName();
                			data << getTile(combatants[1]->getX(), combatants[1]->getY()).getType();

                			// Swapping to the combat state
        				stateManager.pushState(StateManager::COMBAT, &data);

        				// Cleaning up

        				combatants[0] = NULL;
        				combatants[1] = NULL;
        			}
            			selectedUnit->getSprite().setColor(sf::Color(43,43,43));
    				selectedUnit->setMoved(true);
    				deselectUnit(ui, false);
    				break;
			}

			previouslyHoveredTile = hoveredTile;
		}

		// Checking to see if the turn has ended
		bool next = true;
		for(auto &unit : combatController.getEnemyUnits())
		{
			if(!unit.getMoved())
			{
				next = false;
				break;
			}
		}

		if(next)
			nextTurn();
	}
	combatController.updateAnimations(frameTime);
}

// Draw method, draws the tiles and the AI-controlled units
void Level::draw(sf::RenderWindow& window)
{
    // Unit iterator
    std::list<Unit>::iterator cUnitItr;

    // Drawing the tiles
    for(int i = 0; i < levelWidth; i++)
    {
        for(int j = 0; j < levelHeight; j++)
        {
            window.draw(tiles[i][j].getSprite());
        }
    }

	// Drawing the enemy units
	for(auto &unit : combatController.getAvailableUnits())
		window.draw(unit.getSprite());

	// Drawing the player units
	for(auto &unit : combatController.getEnemyUnits())
		window.draw(unit.getSprite());
}

std::string Level::getTileType(int x, int y)
{
    // Preventing horrible things from happening
    if(x < levelWidth && y < levelHeight && x >= 0 && y >= 0)
    {
        return tiles[x][y].getType();
    }
    else
    {
        // Tile specified doesn't exist, returning a large value so that the
        // tile is ignored by the pathfinder.
        return "wall";
    }
}

void Level::nextTurn()
{
	if(playerTurn)
	{
		playerTurn = false;
		for(auto &unit : combatController.getAvailableUnits())
		{
			unit.setMoved(false);
			unit.getSprite().setColor(sf::Color::White);
		}
	}
	else
	{
		playerTurn = true;
		for(auto &unit : combatController.getEnemyUnits())
		{
			unit.setMoved(false);
			unit.getSprite().setColor(sf::Color::White);
		}

		turnState = TurnState::SELECTION;
	}
}

void Level::deselectUnit(GameUserInterface& ui, bool moveBack)
{
	// Clearing highlights
	ui.clearHighlight();
	toHighlight.clear();
	toHighlightAtk.clear();

	// Moving unit back
	if(moveBack)
		selectedUnit->setPosition((sf::Vector2f)selectedUnitPos, tileSize);

	selectedUnitPos = sf::Vector2i(-1,-1);
	selectedUnit = NULL;
	turnState = TurnState::SELECTION;
}

Tile Level::getTile(int x, int y) {return tiles[x][y];}
int Level::getMapSizeX(){ return levelWidth; }
int Level::getMapSizeY(){ return levelHeight; }

Level::~Level()
{
    // Clearing up the tile array
    for(int i = 0; i < levelWidth; ++i)
        delete[] tiles[i];
    delete tiles;
}
