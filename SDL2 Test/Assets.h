#pragma once
#include "includes.h"

class Asset 
{
	public:
		Asset(SDL_Texture* texture, int w, int h) {
			this->texture = texture;
			this->w = w;
			this->h = h;
		};

		SDL_Texture* texture;
		int w;
		int h;
};

class Assets
{
public:
	static Asset* getAsset(std::string path, SDL_Renderer* renderer);
};

