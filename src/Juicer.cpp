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
				case JuiceType::RMOVE:
				{
					// Adding a bit of a delay to the slide in
					if(i.timer.getElapsedTime().asMilliseconds() < 250)
						i.object->setPosition(i.target.x - 100, i.target.y);
					else if(i.object->getPosition().x < i.target.x)
						i.object->move(5.f, 0.f);
					break;
				}
				case JuiceType::BOUNCE:
				{
					i.object->move(0.f, -1.f);
					break;
				}
			}
			bitmask &= ~mask;
			mask <<= 1;
		}

		mask = 1;
	}
}
