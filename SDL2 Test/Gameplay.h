#pragma once
#include "Menu.h"
#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "Level.h"
#include "Wall.h"
#include "Camera.h"
class Gameplay :
    public Menu
{
	public:
		Gameplay();
		void update(Events::updateEvent event) override;

		static TextDisplay* getHud();

		static void onLoggedIn();
		void reloadLevel();

		static void onUpdateGameState();

		Level* currentLevel;

		static Player* getLocalPlayer();
		static Entity findEntityById(unsigned int);
		static Player* findPlayerById(unsigned int);
		static Player* createPlayer(Entity en);


};

