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
#include <SFML/System/Clock.hpp>
#include <vector>

class Juicer
{
	public:
		enum JuiceType
		{
			LERP, QUAD_EASE_OUT, QUAD_EASE_IN, QUAD_EASE_IN_OUT,
			ELASTIC_OUT, ELASTIC_IN_OUT
		};

		enum TRS
		{
			TRANSLATE = 1,
			SCALE = 2,
			ROTATE = 3
		};

		Juicer();
		~Juicer();
		void add(sf::Transformable* obj, JuiceType jt, TRS trs,
				 sf::Time length, float start, float end);
		void add(sf::Transformable* obj, JuiceType jt, TRS trs,
				 sf::Time length, sf::Vector2f start, sf::Vector2f end);
		void remove(sf::Transformable* obj);
		void update();

	private:
		struct JuiceGroup
		{
			sf::Transformable* object;
			JuiceType jt;
			TRS trs;
			sf::Clock timer;
			sf::Time length;
			sf::Vector2f v1;
			sf::Vector2f v2;
			
			JuiceGroup (sf::Transformable* object, JuiceType jt, TRS trs,
						sf::Time length, float start, float end)
			{
				this->object = object;
				this->jt = jt;
				this->trs = trs;
				this->length = length;
				this->v1.x = start;
				this->v1.y = end;
			}

			JuiceGroup (sf::Transformable* object, JuiceType jt, TRS trs,
						sf::Time length, sf::Vector2f start, sf::Vector2f end)
			{
				this->object = object;
				this->jt = jt;
				this->trs = trs;
				this->length = length;
				this->v1 = start;
				this->v2 = end;

			}
		};

		void ease(float& r, JuiceType jt);

		std::vector<JuiceGroup> vec;
};
#endif//JUICER_H
