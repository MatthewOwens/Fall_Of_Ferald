#include "Unit.h"
#include <iostream>

#ifndef __GNUC__
#endif

const std::string Unit::statNames[9] = {"health", "strength", "magic", "skill",
								 "speed", "defense", "resistance", "luck",
								 "moveRange"};

// Constructor
Unit::Unit(std::string unitName, std::string unitType, int lvl, int health, int strength, int magic, int skill,
	int speed, int defense, int resistance, int luck, int moveRange, int _x, int _y)
{
	level = lvl;
	moved = false;

    // Initilizing the unit's stats
    stats["health"] = Stat(health);
    stats["strength"] = Stat(strength);
	stats["magic"] = Stat(magic);
    stats["skill"] = Stat(skill);
    stats["speed"] = Stat(speed);
    stats["defense"] = Stat(defense);
	stats["resistance"] = Stat(resistance);
	stats["luck"] = Stat(luck);
    stats["moveRange"] = Stat(moveRange);
	movementType = "foot";

    if (unitName == "")
        name = unitType;
    else name = unitName;
	//name = unitName;

	std::cout << "Created a unit called " << name << "!" << std::endl;

	x = _x;
	y = _y;
}

// Alternative constructor for a generic unit, takes an array
Unit::Unit(std::string unitType, int _x, int _y, int* statArray, int lvl)
{
    // Naming the unit based on it's type.
    name = unitType;
    type = unitType;

    // Setting the unit's level
    level = lvl;
	moved = false;

	for(int i = 0; i < getStatsSize(); ++i)
		stats[statNames[i]] = Stat(statArray[i]);

	x = _x;
	y = _y;
}

Unit::~Unit()
{
    //dtor
}

void Unit::modifyStat(std::string statName, int mod)
{
    // Ensuring that the unit cannot be healed over it's max health
    if(statName == "health" && mod < 0)   // if the modifier is less than zero, healing is occuring.
    {
        if (stats[statName].getCurrent() + mod < stats[statName].base)
            stats[statName].modifier += mod;
        else
            stats[statName].modifier = 0;
    }
    else
    {
        stats[statName].modifier += mod;
    }
}

std::string Unit::getHighestStat()
{
    std::map<std::string, Stat>::iterator stat_itr;
    std::string highestName;
    int highestVal = 0;

    for(stat_itr = stats.begin(); stat_itr != stats.end(); ++stat_itr)
    {
        if((stat_itr->second.getCurrent() > highestVal)
           && (stat_itr->first == "magic" || stat_itr->first == "strength" || stat_itr->first == "defense"))
        {
            highestVal = stat_itr->second.getCurrent();
            highestName = stat_itr->first;
        }
    }
    return highestName;
}

int Unit::getStat(std::string statName)
{
    return stats[statName].getCurrent();
}

int Unit::getMaxRange()
{
	return 1;
}

void Unit::setPosition(int newX, int newY, int tileSize)
{
	// Setting the unit's position on the grid
	x = newX;
	y = newY;

	// Setting the sprite's position
	sprite.setPosition(x * tileSize, y * tileSize);
}

void Unit::setPosition(sf::Vector2f newPos, int tileSize)
{
	// Setting the unit's position on the grid
	x = newPos.x;
	y = newPos.y;

	// Setting the sprite's position
	sprite.setPosition(x * tileSize, y * tileSize);
}

std::string Unit::getInfo()
{
	std::string result = "";
    std::map<std::string, Stat>::iterator stat_itr;

    for(stat_itr = stats.begin(); stat_itr != stats.end(); ++stat_itr)
    {
		if(stat_itr->first != "moveRange")
			result += stat_itr->first + ": " + std::to_string(stat_itr->second.getCurrent()) + "\n";
	}

	return result;

}

bool Unit::useItem(std::string itemName)
{
	Item* item = inventory.getItem(itemName);

	// Ensuring that the item exists
	if(item != nullptr)
	{
		std::cout << "Removing " << item->getName() << " from inventory." << std::endl;
	   	std::cout << "Decreasing " << item->getTargetStat() << " by " << item->getModifier() << std::endl;
		stats[item->getTargetStat()].modifier += item->getModifier();

		// Removing the item from our inventory
		inventory.removeItem(itemName);
		return true;
	}
	else
	{
		std::cout << "Item " << itemName << " not found in " << getName() << "'s inventory!" << std::endl;
		std::cout << item << " was returned!" << std::endl << std::endl;
		return false;
	}
}
