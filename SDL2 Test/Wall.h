#pragma once
#include "Object.h"
#include "includes.h"
class Wall :
    public Object
{
	public:
		Wall(float x, float y) : Object(x, y) {
			type = Wall_e;
		};
		Wall() = default;
		~Wall() = default;
		void update(Events::updateEvent event) override;

		SDL_FRect rect;
};

