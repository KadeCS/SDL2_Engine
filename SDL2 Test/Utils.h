#pragma once
#include <SDL_ttf.h>
class Utils
{
	public:
		static float clamp(float n, float lower, float upper);
		static TTF_Font* getFont();
};

