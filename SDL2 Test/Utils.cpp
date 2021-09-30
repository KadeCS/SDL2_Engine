#include "Utils.h"


float Utils::clamp(float n, float lower, float upper)
{
	if (n < lower)
		return lower;
	if (n > upper)
		return upper;
	return n;
}


TTF_Font* Utils::getFont()
{
	static TTF_Font* Sans = nullptr;

	if (!Sans)
		Sans = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 24);

	return Sans;
}