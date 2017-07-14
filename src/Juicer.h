/// Juicer is a generic class that makes SFML Transformables 'juicy',
/// associating the object with a function and an optional lifetime to make the
/// object move/shake etc.
///
/// This is accomplished by associating a sf::Transformable with a function
/// to transform the object. The Juicer does not own any of the transformables
/// simply adding/removing them from a vector as needed.

#ifndef JUICER_H
#define JUICER_H
#include <SFML/Graphics/Transformable.hpp>
#include <vector>

class Juicer
{
	public:
		Juicer();
		~Juicer();
		void add(sf::Transformable* obj,int bitmask);
		void remove(sf::Transformable* obj);
		void update();
		sf::Vector2f testMove(const sf::Vector2f& pos);

		//sf::Vector2f testMove(const sf::Vector2f& pos);
		enum JuiceType
		{
			LMOVE = 1,
			BOUNCE = 2
		};

	private:
		int mask = 1;

		struct Group
		{
			sf::Transformable* object;
			int bitmask;

			Group(sf::Transformable* object, int bitmask)
			{
				this->object = object;
				this->bitmask = bitmask;
			}
		};

		std::vector<Group> vec;
};
#endif//JUICER_H
