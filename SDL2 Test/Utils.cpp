#include "Utils.h"
#include <complex>


float Utils::clamp(float n, float lower, float upper)
{
	if (n < lower)
		return lower;
	if (n > upper)
		return upper;
	return n;
}

float Utils::lerp(float __a, float __b, float __t) // stolen from cmath cuz I don't have C++20
{
	if (__a <= 0 && __b >= 0 || __a >= 0 && __b <= 0)
		return __t * __b + (1 - __t) * __a;

	if (__t == 1)
		return __b; 
	const float __x = __a + __t * (__b - __a);
	return __t > 1 == __b > __a
		? (__b < __x ? __x : __b)
		: (__b > __x ? __x : __b);
}

float Utils::Align(float value, float size)
{
	return value - std::abs(std::fmod(value, size));
}


TTF_Font* Utils::getFont()
{
	static TTF_Font* Sans = nullptr;

	if (!Sans)
		Sans = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 24);

	return Sans;
}