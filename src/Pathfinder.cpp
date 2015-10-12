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

// Calculates the area that a unit can move to and attack, takes a reference to a unit
// and references to moveSet and atkSet to modify.
void Pathfinder::calculateArea(Unit& unit, std::vector<sf::Vector3i>& moveSet, std::vector<sf::Vector2i>& atkSet)
{
	int moveRange = unit.getStat("moveRange");
	std::string moveType = unit.getMovementType();
	int atkRange = moveRange + 2;					// TODO: Change to += weapon range
	sf::Vector2i startPos(unit.getX(), unit.getY());
	std::list<sf::Vector3i> openSet;
	//std::vector<sf::Vector3i> openSet;
	int cornerSize = 1;

	// No need to go further if the unit can't move
	if(moveRange <= 0)
	{
		// Since if we're here, it's probably accidental
		std::cout << unit.getName()  << " can't move!" << std::endl;
		return;
	}

	// Ensuring everything's empty.
	moveSet.clear();
	atkSet.clear();

	// Calculating how large the corners of the range are
	for(int i = 2; i < atkRange; ++i)
		cornerSize += i;

	// Calculating the total size of our range
	const int maxSearchSize = 1 + (4 * atkRange) + (4 * cornerSize);

	// Pushing our start point to the open set
	openSet.push_back(sf::Vector3i(unit.getX(), unit.getY(), 0));

	auto currentNode = openSet.begin();
	while(openSet.size() != maxSearchSize)
	{
		sf::Vector3i adjacentNodes[4];

		// Populating the adjacent nodes
		std::cout << std::endl;
		std::cout << "Current node: (" << currentNode->x << "," << currentNode->y << "," << currentNode->z << ")" << std::endl;
		std::cout << "populating adjacentNodes" << std::endl;
		adjacentNodes[0] = sf::Vector3i(currentNode->x + 1, currentNode->y, 9999);
		adjacentNodes[1] = sf::Vector3i(currentNode->x - 1, currentNode->y, 9999);
		adjacentNodes[2] = sf::Vector3i(currentNode->x, currentNode->y + 1, 9999);
		adjacentNodes[3] = sf::Vector3i(currentNode->x, currentNode->y - 1, 9999);

		for(int i = 0; i < 4; ++i)
		{
			// Preventing us from reading outside of the level's tile array
			if(adjacentNodes[i].x >= 0 && adjacentNodes[i].x < levelPtr->getMapSizeX() &&
			   adjacentNodes[i].y >= 0 && adjacentNodes[i].y < levelPtr->getMapSizeY())
			{
				adjacentNodes[i].z = moveCosts[moveType][levelPtr->getTileType(adjacentNodes[i].x, adjacentNodes[i].y)];
				adjacentNodes[i].z += currentNode->z;
			}
			std::cout << i << ": (" << adjacentNodes[i].x << "," << adjacentNodes[i].y << "," << adjacentNodes[i].z << ")" << std::endl;

			auto node = openSet.begin();

			// Checking if the adjacent nodes are cheaper than any currently
			// existing nodes in openSet
			do
			{
				if(node->x == adjacentNodes[i].x && node->y == adjacentNodes[i].y)
				{
					if(node->z > adjacentNodes[i].z)
					{
						node->z = adjacentNodes[i].z;
						break;
					}

				}
				node++;
			}while(node != openSet.end());

			// If the node doesn't exist at all in openSet
			if(node == openSet.end())
				openSet.push_back(adjacentNodes[i]);
		}

		std::cout << "Current openSet: " << std::endl;
		for(auto i : openSet)
			std::cout << "\t(" << i.x << "," << i.y << "," << i.z << ")" << std::endl;

		std::cout << "Current node address: " << &currentNode << std::endl;

		currentNode++;
	}

	//std::cout << "openSet: " << std::endl;

	// Culling nodes that are too expensive or off the map
	for(auto i = openSet.begin(); i != openSet.end() ; )
	{
		//std::cout << "\t(" << i->x << "," << i->y << "," << i->z << ")" << std::endl;
		if(i->x < 0 || i->x > levelPtr->getMapSizeX() || i->y < 0 ||
				i->y > levelPtr->getMapSizeY() || i->z > atkRange)
		{
			i = openSet.erase(i);
		}

		++i;
	} 


	std::cout << std::endl << std::endl;
	std::cout  << "Final openSet: " << std::endl;
	// Populating our final vectors
	for(auto i : openSet)
	{
		std::cout << "\t(" << i.x << "," << i.y << "," << i.z << ")" << std::endl;
		/*if(i.z <= moveRange)
			moveSet.push_back(i);
		else if(i.z <= atkRange)
			atkSet.push_back(sf::Vector2i(i.x, i.y));*/
		moveSet.push_back(i);
	}
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

	return finalPath;
}
