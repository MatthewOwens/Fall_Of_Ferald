/* Does what it sais on the tin, used to find the shortest path between two
 * tiles and find targets within a range. Used by both the AI class for
 * calculations and the interface class for highlighting a unit's route during
 * the player's movement phase */

#ifndef PATHFINDER_H
#define PATHFINDER_H
#include <vector>
#include <list>
#include <SFML/System/Vector2.hpp>
#include "Unit.h"
#include <stack>

class Level;

class Pathfinder
{
    public:
        Pathfinder(Level* currentLevel);
		Pathfinder() {}
        virtual ~Pathfinder();
        std::vector<sf::Vector3i> calculateArea(sf::Vector2i start, int range, const std::string& moveType = "flight");
        void calculateArea(Unit& unit, std::vector<sf::Vector3i>& moveSet, std::vector<sf::Vector2i>& atkSet, bool playerUnit = false);
        std::vector<sf::Vector2i> getPath(Unit& currentUnit);
		std::stack<sf::Vector2i> getPath(std::vector<sf::Vector3i> searchRange, sf::Vector2i start, sf::Vector2i target);
        void findPlayerPaths(std::list<Unit>& playerUnits);
        void clearPaths();
    private:
        // Map for storing movement costs relative to a unit type, accessed
        // with [moveType][tileType]
        std::map<std::string, std::map<std::string, int> > moveCosts;

        // Stored priority queues representing the paths for Player-controlled
        // units, stops them from being recalculated during down-time.
        std::vector<std::vector<sf::Vector2i>> playerPaths;
        Level* levelPtr;
	public:
		void findMoveRange(Unit& unit, std::vector<sf::Vector3i>& moveSet, bool playerUnit);
		void findAtkRange(const sf::Vector2i& start, std::vector<sf::Vector3i>& moveSet,
				          std::vector<sf::Vector2i>& atkSet, int atkRange);
		std::vector<sf::Vector2i> findRange(std::string& moveType, int rangeStat, bool isPlayerUnit);
};

#endif // PATHFINDER_H
