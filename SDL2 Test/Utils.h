#pragma once
#include <SDL_ttf.h>
class Utils
{
	public:
		static float clamp(float n, float lower, float upper);
		static float lerp(float __a, float __b, float __t);
		static float Align(float value, float size);
		static TTF_Font* getFont();
};

