#pragma once
#include "Player.h"
#include "Object.h"
#include "TextDisplay.h"
#include <map>

class Game
{
	public:

		void createGame();

		static TextDisplay* getHud();

		void update(Events::updateEvent updateEvent);
		void keyDown(SDL_KeyboardEvent event);
		void keyUp(SDL_KeyboardEvent event);

		static void onLoggedIn();
		static void onUpdateGameState();

		static Player* getLocalPlayer();
		static Entity findEntityById(unsigned int);
		static Player* findPlayerById(unsigned int);
		static Player* createPlayer(Entity en);
		static std::vector<Object*>* getGlobalObjects();
		static void addGlobalObject(Object* obj);
		static void removeGlobalObject(Object* obj);
		static std::map<int, bool> controls;

		static bool getKey(int code);

		static SDL_Renderer* renderer;
		static SDL_Window* window;
		static double gameFPS;
		static float deltaTime;
};

