#ifndef EASING_H
#define EASING_H
#include <cmath>
#define EASING_PI 3.14159265358979323846264338327950288
#define EASING_THI (EASING_PI / 2)
namespace easing
{
	inline
	float lerp(float p)
	{
		return p;
	}

	inline
	float quadraticEaseOut(float p)
	{
		return -(p *(p - 2));
	}

	inline
	float quadraticEaseIn(float p)
	{
		return p * p;
	}

	inline
	float quadraticEaseInOut(float p)
	{
		if (p < .5f)
			return 2 * p * p;
		else
			return (-2 * p * p) + (4 * p) - 1;
	}

	inline
	float elasticOut(float p)
	{
		return sin(-13 * EASING_THI * (p + 1)) * pow(2, -10 * p) + 1;
	}

	inline
	float elasticInOut(float p)
	{
		if(p < 0.5f)
			return 0.5 * sin(13 * EASING_THI * (2 * p)) * pow(2, 10 * ((2 * p) - 1));
		else
			return 0.5 * (sin(-13 * EASING_THI * ((2 * p - 1) + 1)) * pow(2, -10 * (2 * p - 1)) + 2);

	}
}
#endif//EASING_H
