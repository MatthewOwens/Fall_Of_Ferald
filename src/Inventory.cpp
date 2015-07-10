#include "Inventory.h"

void Inventory::addItem(Item newItem)
{
	for(auto existing : stocks)
	{
		if(existing.item.getName() == newItem.getName())
		{
			existing.count++;
			return;
		}
	}

	stocks.push_back(newItem);
}

void Inventory::removeItem(std::string itemName)
{
	std::list<Stock>::iterator stock_itr;
	for(stock_itr = stocks.begin(); stock_itr != stocks.end() ; )
	{
		if(itemName == stock_itr->item.getName())
		{
			// If the stock count is greater than one, we can just decrease the count
			if(stock_itr->count > 1)
			{
				stock_itr->count--;
				return;	// Stock has been removed, no need to continue
			}
			else	// Removing the listing if there is no stock left
			{
				stock_itr = stocks.erase(stock_itr);
			}
		}

		// No match, check the next item in stocks
		++stock_itr;
	}
}

bool Inventory::exists(std::string itemName)
{
	for(auto stock : stocks)
	{
		if(stock.item.getName() == itemName)
			return true;
	}
	return false;
}
