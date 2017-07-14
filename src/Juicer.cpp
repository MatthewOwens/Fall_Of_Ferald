#include "Juicer.h"
#include <iostream>

Juicer::Juicer()
{
}

Juicer::~Juicer()
{
}

void Juicer::add(sf::Transformable* obj, int bitmask)
{
	if(obj != NULL)
		vec.emplace_back(Group(obj, bitmask));
}

void Juicer::remove(sf::Transformable* obj)
{
	for(auto i = vec.begin(); i != vec.end(); ++i)
	{
		if(i->object == obj)
		{
			vec.erase(i);
			break;
		}
	}
}

void Juicer::update()
{
	for(auto i : vec)
	{
		// Copying the bitmask out so it's not consumed for the next frame
		int bitmask = i.bitmask;

		while(bitmask)
		{
			switch(bitmask & mask)
			{
				case JuiceType::LMOVE:
					i.object->move(1.f, 0.f);
					break;
				case JuiceType::BOUNCE:
					i.object->move(0.f, -1.f);
					break;
			}
			bitmask &= ~mask;
			mask <<= 1;
		}

		mask = 1;
	}
}

sf::Vector2f Juicer::testMove(const sf::Vector2f& pos)
{
	auto ret = pos;
	ret.x++;
	return ret;
}
