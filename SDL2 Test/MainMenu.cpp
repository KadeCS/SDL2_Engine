#include "MainMenu.h"
#include "TextBox.h"
#include "Multiplayer.h"
#include "Game.h"
#include "Gameplay.h"

bool accepted = false;

TextBox* textBox;

MainMenu::MainMenu()
{
	textBox = new TextBox(300, 300, "", 100, 30);
	textBox->create();
	textBox->isFocused = true;

	// Connect to the server
	CreateThread(NULL, NULL, Multiplayer::connect, NULL, NULL, NULL);
}

void MainMenu::update(Events::updateEvent event)
{
}

void MainMenu::keyDown(SDL_KeyboardEvent event)
{
	if (event.keysym.sym == SDLK_RETURN && Multiplayer::awaitingLogin)
	{
		if (textBox->text->length() != 0)
		{
			Multiplayer::login(*textBox->text);
			Game::currentMenu = new Gameplay();
			textBox->die();
			delete this;
		}
	}
}
