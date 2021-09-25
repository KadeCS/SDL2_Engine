#pragma once
#include "Player.h"
#include "Object.h"
#include <map>

class Game
{
	public:
		void update(Events::updateEvent updateEvent);
		void keyDown(SDL_KeyboardEvent event);
		void keyUp(SDL_KeyboardEvent event);
		static Player* getPlayer();
		static void addGlobalObject(Object* obj);
		static void removeGlobalObject(Object* obj);
		static std::map<int, bool> controls;

		static bool getKey(int code);

		static SDL_Window* window;
		static int gameFPS;
};

