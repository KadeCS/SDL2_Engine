#pragma once
#include "Player.h"
#include "Object.h"
#include <map>
#include "Menu.h"

class Game
{
	public:

		void createGame();

		void update(Events::updateEvent updateEvent);
		void keyDown(SDL_KeyboardEvent event);
		void keyUp(SDL_KeyboardEvent event);

		void textInput(SDL_TextInputEvent event);


		static Menu* currentMenu;

		static std::vector<Object*>* getGlobalObjects();
		static void addGlobalObject(Object* obj);
		static void removeGlobalObject(Object* obj);
		static std::map<int, bool> controls;

		static bool containsObject(unsigned int id);

		static bool getKey(int code);

		static SDL_Renderer* renderer;
		static SDL_Window* window;
		static float startTick;
		static double gameFPS;
		static float deltaTime;
};

