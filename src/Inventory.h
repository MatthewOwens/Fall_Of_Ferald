#ifndef INVENTORY_H
#define INVENTORY_H
#include "Item.h"
#include <map>
#include <list>

class Inventory
{
	public:
		void addItem(Item newItem);
		void removeItem(std::string itemName);

		struct Stock
		{
			Item item;
			int count;

			Stock(Item& newItem)
			{
				item = newItem;
				count = 1;
			}
		};
	private:
		std::list<Stock> stocks;
};
#endif //INVENTORY_H
