#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item
{
	public:
		// Constructor for weapons & consumables
		Item(std::string name_, std::string targetStat_, int modifier_, int range_, int excludedRange_);

		// Constructor for key items
		Item(std::string name_);

		// Default constructor for inventory
		Item(){};

		// Check functions to determine the type of item
		bool isWeapon();
		bool isKey();
		bool isConsumable();

		// Getters, return const references to reduce memory overhead slightly
		const std::string& getName() { return name; }
		const std::string& getTargetStat() { return targetStat; }
		const int& getModifier() { return modifier; }
		const int getRange() { return range; }
		const int getExcludedRange() { return excludedRange; }
	private:
		std::string name;
		std::string targetStat;
		int modifier;
		int range;
		int excludedRange;
};
#endif //ITEM_H
