#pragma once
#include "Object.h"

class Game
{
	public:
		void update(Events::updateEvent updateEvent);
		void keyDown(SDL_KeyboardEvent event);
		void keyUp(SDL_KeyboardEvent event);
		static void addGlobalObject(Object* obj);
		static void removeGlobalObject(Object* obj);
};

