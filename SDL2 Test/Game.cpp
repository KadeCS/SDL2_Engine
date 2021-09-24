#include "includes.h"
#include "Player.h"
#include "Game.h"
using namespace std;

vector<Object*> objects = {};

Player *p = NULL;


void Game::update(Events::updateEvent update)
{
	SDL_SetWindowTitle(update.window, to_string(SDL_GetTicks()).append(" Ticks have passed").c_str());

	SDL_SetRenderDrawColor(update.renderer, 100, 100, 100, 255);
	SDL_RenderClear(update.renderer);
	SDL_RenderPresent(update.renderer);


	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->update(update);
	}

	// other random code lol

	if (!p)
	{
		// if a player doesn't exist, make one
		p = new Player(0,0);
	}

}




void Game::keyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_w:
			cout << "pressed w\n";
		break;
		case SDLK_s:
			cout << "pressed s\n";
			break;
		case SDLK_d:
			cout << "pressed d\n";
			break;
		case SDLK_a:
			cout << "pressed a\n";
			break;
	}


}

void Game::keyUp(SDL_KeyboardEvent event)
{

}

void Game::addGlobalObject(Object* obj)
{
	objects.push_back(obj);
}

void Game::removeGlobalObject(Object* obj)
{

}
