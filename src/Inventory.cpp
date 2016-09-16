#include "Inventory.h"
#include <iostream>
#include <algorithm>

void Inventory::addItem(Item newItem)
{
	// Adding the new inventory item
	stock.push_back(newItem);

	std::cout << "Added "<< stock.back().getName() << " to the inventory" << std::endl;
}

void Inventory::removeItem(std::string itemName)
{
	stock.erase(std::remove_if(stock.begin(), stock.end(),
				[&](Item i){return i.getName() == itemName;}), stock.end());
}

bool Inventory::exists(std::string itemName)
{
	for(auto item: stock)
	{
		if(item.getName() == itemName)
			return true;
	}
	return false;
}

Item* Inventory::getItem(std::string itemName)
{
	std::vector<Item>::iterator itr;
	for(auto &item : stock)
	{
		std::cout << "Found an item named " << item.getName() << ", looking for " << itemName << std::endl;
		if(item.getName() == itemName)
		{
			std::cout << "Returning " << &item << std::endl;
			return &item;
		}
	}
	return nullptr;
}

