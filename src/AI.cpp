#include "AI.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <deque>

#if defined (__gnu_linux__)
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <errno.h>
	#include <fcntl.h>
#endif

AI::AI(const std::string unitsPath, const std::string statsPath)
{
	populateAIUnits(unitsPath, statsPath);
}

std::list<Unit*> AI::getPossibleTargets(std::vector<sf::Vector3i> attackRange)
{
	std::list<Unit*> possibleTargets;

	for(auto &attackItr : attackRange)
	{
		for(auto &unitItr : enemyUnits)
		{
			if(unitItr.getX() == attackItr.x && unitItr.getY() == attackItr.y)
				possibleTargets.push_back(&unitItr);
		}
	}

	return possibleTargets;
}

std::list<Unit> AI::getPossibleTargets(Unit& currentUnit, Tile** const levelMap)
{
    std::list<Unit>::iterator unitItr;
    std::list<Unit> value;
    int** costs; // The costs to traverse each node in the map
    int range = currentUnit.getMaxRange() + currentUnit.getStat("moveRange");

    // Initilising the costs array
    costs = new int*[mapWidth];
    for(int k = 0; k < mapWidth; ++k)
        costs[k] = new int[mapHeight];

    // Setting the initial costs to -1
    for(int i = 0; i < mapWidth; i++)
        for(int j = 0; j < mapHeight; j++)
            costs[i][j] = -1;

    // Calculate the tile costs for this unit
    calculateCosts(currentUnit, costs, levelMap);

    for(int i = 0; i < mapWidth; i++)
    {
        for(int j = 0; j < mapHeight; j++)
        {
            if(costs[i][j] <= range)
            {
                for(unitItr = enemyUnits.begin(); unitItr != enemyUnits.end(); ++unitItr)
                    if(unitItr->getX() == i && unitItr->getY() == j)
                        value.push_back(*unitItr);
            }
        }
    }

    return value;
}

void AI::calculateCosts(Unit& currentUnit, int** costs, Tile** const levelMap)
{
    std::deque<sf::Vector3i> workingSet;
    workingSet.push_back(sf::Vector3i(currentUnit.getX(), currentUnit.getY(), 0));

    while(!workingSet.empty())
    {
        sf::Vector3i current = workingSet.front();
        sf::Vector3i adjNodes[4];   // Z value represents the cost to travel to this node

        // Populating the adjacent nodes
        adjNodes[0] = sf::Vector3i(current.x + 1, current.y, current.z);
        adjNodes[1] = sf::Vector3i(current.x, current.y - 1, current.z);
        adjNodes[2] = sf::Vector3i(current.x - 1, current.y, current.z);
        adjNodes[3] = sf::Vector3i(current.x, current.y + 1, current.z);

        for(int i = 0; i < 4; i++)
        {
            int cost = 0;//currentUnit.getMoveCost(levelMap[adjNodes[i].x][adjNodes[i].y].getType());

            if(cost != -1)  // If the tile can be traversed
            {
                adjNodes[i].z += cost; // Adding the cost of this tile to the current total

                // If position x,y has a value associated with it
                if(costs[adjNodes[i].x][adjNodes[i].y] != -1)
                {
                    // and this node is cheaper
                    if(costs[adjNodes[i].x][adjNodes[i].y] > adjNodes[i].z)
                        costs[adjNodes[i].x][adjNodes[i].y] = adjNodes[i].z;
                }
                else    // position doesn't have a value associated with it
                    costs[adjNodes[i].x][adjNodes[i].y] = adjNodes[i].z;
            }
        }

        // Finished processing this node, remove it from the working set
        workingSet.pop_front();
    }
}

void AI::updateSprites(const int& tileSize)
{
	for(auto &unit : enemyUnits)
		unit.getSprite().setPosition(unit.getX() * tileSize, unit.getY() * tileSize);

	for(auto &unit : availableUnits)
		unit.getSprite().setPosition(unit.getX() * tileSize, unit.getY() * tileSize);
}

void AI::outputPositions()
{
	for(auto &unit : availableUnits)
		std::cout << unit.getName() << " located at (" << unit.getX() << "," << unit.getY() << ")" << std::endl;

	for(auto &unit : enemyUnits)
		std::cout << "enemy located at (" << unit.getX() << "," << unit.getY() << ")" << std::endl;
}


Unit* AI::selectTarget(std::list<Unit*>& possibleTargets, Unit& currentUnit)
{
	Unit* finalTarget = NULL;
	float heuristic = -1;
	bool killChance;
	bool certainKill;

	for(auto enemy : possibleTargets)
	{
		float hitChance = currentUnit.getStat("skill") - (enemy->getStat("skill") / 2);	// + wep_hitchance
		//float hitChance = currentUnit.getStat("skill") - (enemy.getStat("skill") / 2);	// + wep_hitchance
		float critChance = currentUnit.getStat("skill") / 10;
		//float critChance = currentUnit.getStat("skill") / 10;
		float totalDamage = 10;	// static until weapons get reworked

		// weapon triangle stuff should go here
		
		if(totalDamage > enemy->getStat("health"))
		//if(totalDamage > enemy.getStat("health"))
		{
			killChance = true;
			if(hitChance >= 100)
				certainKill = true;
		}
		
		// Calculating the final heuristic
		float tempHeuristic = 0;
		if(killChance) tempHeuristic += 100;
		if(certainKill) tempHeuristic += 200;
		tempHeuristic += hitChance;
		tempHeuristic += critChance / 4;

		// Selecting the target
		if(tempHeuristic > heuristic)
		{
			finalTarget = enemy;
		}
	}

	return finalTarget;
}

sf::Vector2f AI::selectPosition(std::vector<sf::Vector3i> validRange)
{
	auto bestItr = validRange.begin();

	for(auto itr = validRange.begin(); itr != validRange.end(); ++itr)
	{
		if(itr->z > bestItr->z)
			bestItr = itr;
	}

	return sf::Vector2f(bestItr->x, bestItr->y);
}

void AI::setMapDimensions(int width, int height)
{
    mapWidth = width;
    mapHeight = height;
}

AI::~AI()
{
}

AI::AI()
{
}


void AI::update(Pathfinder& pathfinder, Tile** const tiles, const int& tileSize)
{
	for(auto &unit : getAvailableUnits())
	{
		std::list<Unit*> possibleTargets;		// What the AI controlled unit can attack
		std::vector<sf::Vector3i> moveRange;	// Where the AI controlled unit can move to

		// Finding the moveRange
		moveRange = pathfinder.calculateArea(sf::Vector2i(unit.getX(), unit.getY()), unit.getStat("moveRange"),
				unit.getMovementType());

		// Searching for possible targets based on the moveRange
		possibleTargets = getPossibleTargets(moveRange);

		// If there were valid targets within the unit's attack range
		if(possibleTargets.size() != 0)
		{
			Unit* target = selectTarget(possibleTargets, unit);

			// Ensuring that target isn't null, it shouldn't be but it doesn't hurt to check
			if(target != NULL)
			{
				// Where the AI can attack the target from on the map
				std::vector<sf::Vector3i> validPositions;
				std::vector<sf::Vector3i> tempPositions;

				// calculateArea breaks if the pos vector is constructed in the function call. No idea why.
				sf::Vector2i pos(target->getX(), target->getY());
				tempPositions = pathfinder.calculateArea(pos, 1); // Using 1 'till weapons are done

				// Finding the valid positions
				for(auto &outer : moveRange)
				{
					for(auto &inner : tempPositions)
					{
						if(outer.x == inner.x && outer.y == inner.y)
						{
							int x = inner.x;
							int y = inner.y;
							validPositions.push_back(sf::Vector3i(x, y, tiles[x][y].getTerrainDef()));
						}
					}
				}

				for(auto &itr : availableUnits)
				{
					// Removing spaces containing other available units, but not the current unit
				//	if(itr.getX() != unit.getX() && itr.getY() != unit.getY())
				//	{
						for(auto posItr = validPositions.begin(); posItr != validPositions.end() ; )
						{
							if(posItr->x == itr.getX() && posItr->y == itr.getY())
								posItr = validPositions.erase(posItr);
							else ++posItr;
						}
				/*	}
					else
					{
						std::cout << unit.getType() << " skipping " << itr.getType() << std::endl;
					}
				*/}

				// Removing all spaces containing enemy units
				for(auto &itr : enemyUnits)
				{
					for(auto posItr = validPositions.begin(); posItr != validPositions.end() ; )
					{
						if(posItr->x == itr.getX() && posItr->y == itr.getY())
							posItr = validPositions.erase(posItr);
						else ++posItr;
					}
				}

				// Selecting the best tile to attack from
				if(validPositions.size() != 0)	// Only move if there's a valid position
				{
					sf::Vector2f bestPosition = selectPosition(validPositions);
					unit.setPosition(bestPosition, tileSize);
				}

				// Attacking the target
				target->modifyStat("health", 10);	// 10 is a temp value until weapons are redone

				// Removing the target if we need to
				if(target->getStat("health") <= 0)
				{
					for(auto itr = enemyUnits.begin(); itr != enemyUnits.end() ; )
					{
						if(target == &*itr)
							itr = enemyUnits.erase(itr);
						else ++itr;
					}
				}
			}

			// Updating the unit's sprite
			unit.getSprite().setPosition(unit.getX() * tileSize, unit.getY() * tileSize);
		}
	}
}

bool AI::populateAIUnits(const std::string& unitsPath, const std::string& statsPath)
{

    mapWidth = -1;
    mapHeight = -1;

    // Stuff for file parsing
    std::ifstream unitFile(unitsPath);
    std::ifstream statsFile;
    std::string unitLine;
    std::string unitSubString;
    std::string previousUnitType;
    std::string unitType;
    std::stringstream convert;
    std::stringstream ss;

	int tempX = 0;
    int tempY = 0;
    int tempLvl = 0;

    // Temporary variables to keep track of what line and line section is
    // currently being accessed.
    int i = 0;
    int j = 0;

    // Stuff for randomization
    std::mt19937 gen(std::time(NULL));
    std::uniform_int_distribution<int> dis(1,100);

    if(unitFile.good())
    {
        // For every unit that has to be loaded
        while(std::getline(unitFile,unitLine))
        {
    		// Arrays to store the stats and growths temporarily
  			int baseStats[statCount] =	{0,0,0,0,0,0,0,0,0};
    		int growths[statCount] =	{0,0,0,0,0,0,0,0,0};
    		int finalStats[statCount] = {0,0,0,0,0,0,0,0,0};
	
            ss.str(unitLine);

            // For every parameter in the line
            while(std::getline(ss, unitSubString, ','))
            {
                if (unitSubString[0] != '#')
                {
                    switch(i)
                    {
                    case 0:
                        unitType = unitSubString;
                        break;
                    case 1:
                        convert.str(unitSubString);
                        convert >> tempLvl;
                        break;
                    case 2:
                        convert.str(unitSubString);
                        convert >> tempX;
                        break;
                    case 3:
                        convert.str(unitSubString);
                        convert >> tempY;
                    }
                    // Move to the next line
                    i++;

                    // Clearing the stringstream
                    convert.clear();
                    convert.str(std::string());
                }
            }
            i = 0;

            // Clearing the stringstream
            ss.clear();
            ss.str(std::string());

            // If the current unit type changed from last time, get the
            // stat bases/growths again.
            if(unitType != previousUnitType)
            {
                // Closing the previous stats file
                statsFile.close();

                // Opening a new stats file
                statsFile.open(statsPath + unitType + ".txt");

                if(statsFile.good())
                    std::cout << "The " << unitType << " stats file is good. \n";
                else
				{
                    std::cout << "Error loading " << statsPath + unitType + ".txt" << std::endl;
					availableUnits.clear(); // Clearing to avoid half-populated lists.
					return false;
				}


                while(std::getline(statsFile, unitLine))
                {
                    bool skipLine = false;
                    ss.str(unitLine);

                    while(std::getline(ss,unitSubString,','))
                    {
                        if (unitLine[0] != '#')// || unitLine[0] != ' ')
                        {
                            convert.str(unitSubString);
                            if (i == 0)
                            {
                                convert >> baseStats[j];
                            }
                            else if (i == 1)
                                convert >> growths[j];

                            j++;
                        }
                        else
                        {
                            skipLine = true;
                        }

                        // Clear the stringstream
                        convert.clear();
                        convert.str(std::string());
                    }
                    // Move to the next line
                    j = 0;

                    if (!skipLine)
                    {
                        i++;
                    }

                    ss.clear();
                    ss.str(std::string());
                }
            }
            // Resetting the counters for the next line
            i = 0;

            // leveling up each of the individual stats depending on the unit's level
            for(int k = 0; k < statCount; k++)
            {
                finalStats[k] += baseStats[k];
                int rng;
                for(int j = 0; j < tempLvl - 1; j++)
                {
                    rng = dis(gen);

                    if(rng < growths[k])
                    {
                        finalStats[k]++;
                    }
                }
            }

            // Adding the unit to the available units and assigning the previous unit type
            availableUnits.push_back(Unit(unitType, tempX, tempY, finalStats, tempLvl));
            previousUnitType = unitType;
        }
    }
	else	// unitFile.good == false
	{
		std::cout << "Error reading from " << unitsPath << std::endl;
		availableUnits.clear();	// Clearing to avoid half-populated lists
		return false;
	}

    // Closing the unit file
    unitFile.close();
	return true;
}

std::vector<sf::Vector2i> AI::getItemRange(Unit& currentUnit, const int& attackRange, const int& excludedRange)
{
	std::vector<sf::Vector2i> finalRange;
	int x  = currentUnit.getX();
	int y = currentUnit.getY();
	int moveRange = currentUnit.getStat("moveRange");
	sf::Vector2i location(x, y);

	// Checking for units that can't attack
	if(attackRange <= 0)
		return finalRange;
	else if(excludedRange >= attackRange)
	{
		std::cout << "/!\\ Excluded range > attack range for " << currentUnit.getName() << " /!\\" << std::endl;
	}
	else
	{
		for(int i = 0; i <= attackRange; ++i)
		{
			for(int j = 0; j <= attackRange; ++j)
			{
				// Preventing units from outside the attack range and the current location
				// from being added. 
				if((j + i) <= attackRange && (j + i) != 0 && (i + j) > excludedRange)
				{
					// Adding x and y to shift the end result to be absolute to the map, rather than
					// relative to the unit
					finalRange.push_back(sf::Vector2i(i + x, j + y));

					// Preventing flipping if the point is on an axis, as it would just result in
					// duplications in finalRange.
					if(i != 0)
						finalRange.push_back(sf::Vector2i(-i + x, j + y));
					if(j != 0)
						finalRange.push_back(sf::Vector2i(i + x, -j + y));
					if(i != 0 && j != 0)
						finalRange.push_back(sf::Vector2i(-i + x, -j + y));
				}
			}
		}
	}
	return finalRange;
}
