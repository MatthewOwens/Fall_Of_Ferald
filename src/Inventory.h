#ifndef INVENTORY_H
#define INVENTORY_H
#include "Item.h"
#include <vector>

class Inventory
{
	public:
		Inventory(){};
		void addItem(Item newItem);
		void removeItem(std::string itemName);
		bool exists(std::string itemName);
		Item* getItem(std::string itemName);

	private:
		std::vector<Item> stock;
};
#endif //INVENTORY_H
