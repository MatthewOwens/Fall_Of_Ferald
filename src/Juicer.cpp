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

void Juicer::ease(float& r, JuiceType jt)
{
	switch(jt)
	{
		case LERP:
			r = easing::lerp(r);
			break;
		case QUAD_EASE_OUT:
			r = easing::quadraticEaseOut(r);
			break;
		case QUAD_EASE_IN:
			r = easing::quadraticEaseIn(r);
			break;
		case QUAD_EASE_IN_OUT:
			r = easing::quadraticEaseInOut(r);
			break;
		case ELASTIC_OUT:
			r = easing::elasticOut(r);
			break;
		case ELASTIC_IN_OUT:
			r = easing::elasticInOut(r);
			break;
		default:
			std::cerr << "Jucier: unknown easing function, defaulting to lerp" << std::endl;
	}
}

void Juicer::update()
{
	for(auto itr = vec.begin(); itr != vec.end(); )
	{
		float r = itr->timer.getElapsedTime() / itr->length;
		if(r < 1.f)
		{
			// Applying the easing function
			ease(r, itr->jt);

			switch(itr->trs)
			{
				case TRANSLATE:
				{
					itr->object->setPosition(itr->v1 + ((itr->v2-itr->v1)*r));
					break;
				}
				case SCALE:
				{
					float factor = ((itr->v1.y - itr->v1.x) * r);
					itr->object->setScale(factor,factor);
					break;
				}
				case ROTATE:
				{
					itr->object->setRotation(((itr->v1.y - itr->v1.x) * r) * 360);
					break;
				}
				default:
					std::cerr << "Juicer: unknown TRS" << std::endl;
			}
			++itr;
		}
		else
		{
			// Ensuring that the final positions aren't incorrect due to
			// framerate etc
			switch(itr->trs)
			{
				case TRANSLATE:
					itr->object->setPosition(itr->v2);
					break;
				case SCALE:
					itr->object->setScale(itr->v1.y, itr->v1.y);
					break;
				case ROTATE:
					itr->object->setRotation(itr->v1.y * 360);
					break;
			}
			itr = vec.erase(itr);
		}
	}
}
