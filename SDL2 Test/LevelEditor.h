#pragma once
#include "includes.h"
#include "Menu.h"
#include "Wall.h"
#include "TextDisplay.h"
class LevelEditor :
    public Menu
{
    public:
        LevelEditor() {
            vectorOfWall = new std::vector<Wall*>();
        };



        void update(Events::updateEvent event) override;

        void keyDown(SDL_KeyboardEvent event) override;
        void keyUp(SDL_KeyboardEvent event) override;

        void saveLevel();
		void loadLevel();

        static TextDisplay* getHud();

        static bool pressedM;

        std::vector<Wall*>* vectorOfWall;
};

