#include "Item.h"

Item::Item(std::string name_, std::string targetStat_, int modifier_, int range_, int excludedRange_)
{
	name = name_;
	targetStat = targetStat_;
	modifier = modifier_;
	range = range_;
	excludedRange = excludedRange_;
}

bool Item::isWeapon()
{
	if(targetStat == "health")
		return true;
	else return false;
}

bool Item::isKey()
{
	if(targetStat == "")
		return true;
	else return false;
}

bool Item::isConsumable()
{
	if(!isWeapon() && !isConsumable())
		return true;
	else return false;
}
