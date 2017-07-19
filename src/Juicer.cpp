#include "Juicer.h"
#include "Easing.h"
#include <iostream>

Juicer::Juicer()
{
}

Juicer::~Juicer()
{
}

void Juicer::add(sf::Transformable* obj, JuiceType jt, TRS trs,
				 sf::Time length, float start, float end)
{
	//TODO: add in translate, scale, rotate order
	if(obj != NULL)
		vec.push_back(JuiceGroup(obj, jt, trs, length, start, end));
}
void Juicer::add(sf::Transformable* obj, JuiceType jt, TRS trs,
				 sf::Time length, sf::Vector2f start, sf::Vector2f end)
{
	//TODO: add in translate, scale, rotate order
	if(obj != NULL)
		vec.emplace_back(JuiceGroup(obj, jt, trs, length, start, end));
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
	for(auto itr = vec.begin(); itr != vec.end(); )
	{
		float r = itr->timer.getElapsedTime() / itr->length;
		if(r < 1.f)
		{
			switch(itr->jt)
			{
				case LERP:
				{
					switch(itr->trs)
					{
						case TRANSLATE:
							itr->object->setPosition(itr->v1 + ((itr->v2-itr->v1)*r));
							break;
						case SCALE:
						{
							float factor = ((itr->v1.y - itr->v1.x) * r);
							itr->object->setScale(factor,factor);
						}
							break;
						case ROTATE:
							itr->object->setRotation(((itr->v1.y - itr->v1.x) * r) * 360);
							break;
					}
					break;
				}
				default:
					std::cerr << "Unknown juice type" << std::endl;
			}
			++itr;
		}
		else itr = vec.erase(itr);
	}
}

//void Juicer::update()
//{
//	for(auto i : vec)
//	{
//		// Copying the bitmask out so it's not consumed for the next frame
//		int bitmask = i.bitmask;
//
//		while(bitmask)
//		{
//			switch(bitmask & mask)
//			{
//				case JuiceType::RMOVE:
//				{
//					// Adding a bit of a delay to the slide in
//					if(i.timer.getElapsedTime().asMilliseconds() < 250)
//						i.object->setPosition(i.target.x - 100, i.target.y);
//					else if(i.object->getPosition().x < i.target.x)
//						i.object->move(5.f, 0.f);
//					break;
//				}
//				case JuiceType::BOUNCE:
//				{
//					i.object->move(0.f, -1.f);
//					break;
//				}
//			}
//			bitmask &= ~mask;
//			mask <<= 1;
//		}
//
//		mask = 1;
//	}
//}
