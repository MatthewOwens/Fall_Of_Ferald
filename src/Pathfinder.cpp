#include "Pathfinder.h"
#include "Level.h"

Pathfinder::Pathfinder(Level* currentLevel)
//Pathfinder::Pathfinder()
{
    // Initilising movement costs
    moveCosts["foot"]["plain"] = 1;
    moveCosts["foot"]["forest"] = 2;
    moveCosts["foot"]["mountain"] = 3;
    moveCosts["foot"]["water"] = 999;
    moveCosts["foot"]["wall"] = 999;

    moveCosts["flight"]["plain"] = 1;
    moveCosts["flight"]["forest"] = 1;
    moveCosts["flight"]["mountain"] = 1;
    moveCosts["flight"]["water"] = 1;
    moveCosts["flight"]["wall"] = 999;

    moveCosts["naval"]["plain"] = 999;
    moveCosts["naval"]["forest"] = 999;
    moveCosts["naval"]["mountain"] = 999;
    moveCosts["naval"]["water"] = 1;
    moveCosts["naval"]["wall"] = 999;

    moveCosts["mech"]["plain"] = 1;
    moveCosts["mech"]["forest"] = 1;
    moveCosts["mech"]["mountain"] = 999;
    moveCosts["mech"]["water"] = 999;
    moveCosts["mech"]["wall"] = 999;

    levelPtr = currentLevel;
}

Pathfinder::~Pathfinder()
{
    levelPtr = NULL;
}

void Pathfinder::findMoveRange(Unit& unit, std::vector<sf::Vector3i>& moveSet, bool playerUnit)
{
	int moveRange = unit.getStat("moveRange");
	std::string moveType = unit.getMovementType();
	//int atkRange = 3;					// TODO: Change to += weapon range

	std::deque<sf::Vector3i> openSet;

	openSet.push_back(sf::Vector3i(unit.getX(), unit.getY(), 0));

	std::list<Unit> impassableUnits;
	std::list<Unit> friendlyUnits;

	if(playerUnit)
	{
		impassableUnits = levelPtr->getAI().getAvailableUnits();
		friendlyUnits = levelPtr->getAI().getEnemyUnits();
	}
	else
	{
		impassableUnits = levelPtr->getAI().getEnemyUnits();
		friendlyUnits = levelPtr->getAI().getAvailableUnits();
	}

	while(!openSet.empty())
	{
		sf::Vector3i neighbours[4];
		neighbours[0] = sf::Vector3i(openSet.front().x + 1 , openSet.front().y, 0);
		neighbours[1] = sf::Vector3i(openSet.front().x - 1, openSet.front().y, 0);
		neighbours[2] = sf::Vector3i(openSet.front().x , openSet.front().y + 1, 0);
		neighbours[3] = sf::Vector3i(openSet.front().x , openSet.front().y - 1, 0);

		for (auto i : neighbours)
		{
			// Preventing us checking off the tile array
			if(i.x >= 0 && i.x < levelPtr->getMapSizeX() &&
			   i.y >= 0 && i.y < levelPtr->getMapSizeY() )
			{
				// Getting the cost to move to the neighbour
				i.z = openSet.front().z + moveCosts[moveType][levelPtr->getTiles()[i.x][i.y].getType()];

				if(i.z > moveRange)
					goto nextNeighbour;

				// Is the neighbour's cost cheaper than a matching node in the open set?
				for(auto j : openSet)
				{
					if(i.x == j.x && i.y == j.y)
					{
						if(i.z < j.z)
							j.z = i.z;

						goto nextNeighbour;
					}
				}

				for(auto j : moveSet)
				{
					if(i.x == j.x && i.y == j.y)
						goto nextNeighbour;
				}

				// Checking to see if another unit occupies the neighbour
				for(auto j : impassableUnits)
				{
					if(i.x == j.getX() && i.y == j.getY())
						goto nextNeighbour;
				}

				// All checks passed - adding to the open set
				openSet.push_back(i);
			}
			nextNeighbour:;
		}

		moveSet.push_back(openSet.front());
		openSet.pop_front();
	}

	// Removing the unit's starting position from the set
	moveSet.erase(moveSet.begin());
}

void Pathfinder::findAtkRange(const sf::Vector2i& start, std::vector<sf::Vector3i>& moveSet, std::vector<sf::Vector2i>& atkSet, int atkRange)
{
	std::deque<sf::Vector3i> openSet;

	for(auto i : moveSet)
	{
		openSet.clear();
		openSet.push_back(i);
		openSet.back().z = 0;

		while(!openSet.empty())
		{
			// Finding neighbours
			sf::Vector3i neighbours[4];
			neighbours[0] = sf::Vector3i(openSet.front().x + 1 , openSet.front().y, openSet.front().z + 1);
			neighbours[1] = sf::Vector3i(openSet.front().x - 1, openSet.front().y,  openSet.front().z + 1);
			neighbours[2] = sf::Vector3i(openSet.front().x , openSet.front().y + 1, openSet.front().z + 1);
			neighbours[3] = sf::Vector3i(openSet.front().x , openSet.front().y - 1, openSet.front().z + 1);

			for(auto j : neighbours)
			{
				if(j.z <= atkRange)
				{
					bool alreadyExists = false;

					for(auto k : openSet)
					{
						if(j.x == k.x && j.y == k.y)
						{
							alreadyExists = true;
							break;
						}
					}

					//for(auto k : atkSet)
					//{
					//	if(j.x == k.x && j.y == k.y)
					//	{
					//		alreadyExists = true;
					//		break;
					//	}
					//}

					if(!alreadyExists)
					{
						openSet.push_back(j);
					}
				}
			}

			bool alreadyExists = false;
			if(openSet.front().x == start.x && openSet.front().y == start.y)
				alreadyExists = true;

			if(!alreadyExists)
			{
				for(auto j : atkSet)
				{
					if(j.x == openSet.front().x && j.y == openSet.front().y)
					{
						alreadyExists = true;
						break;
					}
				}
			}

			if(!alreadyExists)
			{
				for(auto j : moveSet)
				{
					if(j.x == openSet.front().x && j.y == openSet.front().y)
					{
						alreadyExists = true;
						break;
					}
				}
			}

			if(!alreadyExists)
				atkSet.push_back(sf::Vector2i(openSet.front().x, openSet.front().y));
			openSet.pop_front();
		}
	}
}

// Calculates the area that a unit can move to and attack, takes a reference to a unit
// and references to moveSet and atkSet to modify.
void Pathfinder::calculateArea(Unit& unit, std::vector<sf::Vector3i>& moveSet,
							   std::vector<sf::Vector2i>& atkSet, bool playerUnit)
{
	int atkRadius;
	if(playerUnit)
		atkRadius = 5;
	else atkRadius = 1;

	findMoveRange(unit, moveSet, playerUnit);
	findAtkRange(unit.getGridPos(), moveSet, atkSet, atkRadius);	// TODO: change to proper attack range value
}

// Calculates the area on the map that a unit can move to, based on it's movement
// range.
// Algorithm could be improved as excess nodes that are added to the expansion
// need to be removed at the end. However if currentIndex was incremented to
// match the number of nodes being pushed to expansion, nodes would be skipped.

// TODO: Change function to take a unit reference rather than the start and range values
//		 This will both cut down on the number of parameters and allow proper terrain
//		 cost evaluation.
std::vector<sf::Vector3i> Pathfinder::calculateArea(sf::Vector2i start, int range, const std::string& moveType)
{
    std::vector<sf::Vector3i> expansion;
    int maxSize = 1;
    int currentIndex = 0;

    // Finding the max size that we need to explore
    while(range != 0)
    {
        maxSize += 4 * range;
        range--;
    }

    expansion.push_back(sf::Vector3i(start.x, start.y, 9999));

    // Defining the area that we need to work with
    while(currentIndex < maxSize)
    {
        sf::Vector3i currentNode;
        sf::Vector3i adjacentNodes[4];

        // Storing the current node to stop multiple accesses to the vector
        currentNode = expansion[currentIndex];

        // Getting the adjacent Nodes
        adjacentNodes[0] = sf::Vector3i(currentNode.x + 1, currentNode.y, 9999);
        adjacentNodes[1] = sf::Vector3i(currentNode.x - 1, currentNode.y, 9999);
        adjacentNodes[2] = sf::Vector3i(currentNode.x, currentNode.y + 1, 9999);
        adjacentNodes[3] = sf::Vector3i(currentNode.x, currentNode.y - 1, 9999);

        // Pushing the adjacent nodes to the expansion if they're unique
        for(int i = 0; i < 4; i++)
        {
            if(std::find(expansion.begin(), expansion.end(), adjacentNodes[i]) == expansion.end())
                expansion.push_back(adjacentNodes[i]);
        }

        currentIndex++;
    }

    // Trimming the excess
    expansion.erase(expansion.begin() + maxSize, expansion.end());

    // Removing non-existant nodes
    for(auto itr = expansion.begin(); itr != expansion.end(); )
    {
        if(itr->x < 0 || itr->x >= levelPtr->getMapSizeX() || itr->y < 0 || itr->y >= levelPtr->getMapSizeY())
        {
            itr = expansion.erase(itr);
        } else ++itr;
    }

	expansion[0].z = 0;

	// Calculating the cost of traveling to any of the nodes from the start node.
	// Starting at one after the start to avoid the start node, as we've set it's cost to zero.
	for(auto i = expansion.begin() + 1; i != expansion.end(); )
	{
		// TODO: Proper move cost evaluation
		i->z = moveCosts[moveType][levelPtr->getTileType(i->x, i->y)];
		//i->z = moveCosts["foot"][levelPtr->getTileType(i->x, i->y)];
		//i->z = moveCosts["foot"]["plain"];
		i->z += std::abs(start.x - i->x);
		i->z += std::abs(start.y - i->y);
		i->z -= 1;

		// Removing the node if it's cost is too high
		if(i->z > maxSize)
		{
			i = expansion.erase(i);
		}
		else ++i;
	}

    return expansion;
}

std::stack<sf::Vector2i> Pathfinder::getPath(std::vector<sf::Vector3i> searchRange, sf::Vector2i start, sf::Vector2i target)
{
	// Using the traceback portion of the lee algorithm to find the final path
	// assuming foot move costs, for this portion.
	std::stack<sf::Vector2i> finalPath;
	std::vector<sf::Vector3i> adjacentNodes;
	int cheapest;

	// Returning if the search range is insufficiant
	if(searchRange.empty())
		return finalPath;

	// Pushing the start node
	finalPath.push(target);
	searchRange.push_back(sf::Vector3i(start.x, start.y, 0));

	while(finalPath.top() != start)
	{
		// Storing the current node to prevent calls to top()
		sf::Vector2i current = finalPath.top();

		// Resetting the cheapest node
		cheapest = 0;

		// Populating adjacent nodes
		for(auto i = searchRange.begin(); i != searchRange.end(); ++i)
		{
			if(i->x == current.x + 1 && i->y == current.y)
				adjacentNodes.push_back(*i);
			else if (i->x == current.x - 1 && i->y == current.y)
				adjacentNodes.push_back(*i);
			else if (i->x == current.x && i->y == current.y + 1)
				adjacentNodes.push_back(*i);
			else if (i->x == current.x && i->y == current.y - 1)
				adjacentNodes.push_back(*i);
		}

		// Finding the cheapest node
		if(adjacentNodes.size() != 0)
		{
			for(int i = 0; i < adjacentNodes.size(); ++i)
			{
				if(adjacentNodes[i].z < adjacentNodes[cheapest].z)
					cheapest = i;
			}

			// Pushing the cheapest node to the stack
			finalPath.push(sf::Vector2i(adjacentNodes[cheapest].x, adjacentNodes[cheapest].y));
		}
	}

	//finalPath.pop();
	return finalPath;
}
