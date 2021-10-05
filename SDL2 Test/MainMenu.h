#pragma once
#include "Menu.h"
class MainMenu :
    public Menu
{
	public:
		MainMenu();
		void update(Events::updateEvent event) override;
		void keyDown(SDL_KeyboardEvent event) override;
};

