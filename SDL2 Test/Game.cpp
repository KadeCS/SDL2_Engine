#include "includes.h"
#include <algorithm>
#include "Player.h"
#include "Game.h"
using namespace std;

vector<Object*> objects = {};

map<int, bool> Game::controls = {
	{SDLK_w, false},
	{SDLK_s, false},
	{SDLK_a, false},
	{SDLK_d, false},
};

SDL_Window* Game::window;

int Game::gameFPS;

void Game::update(Events::updateEvent update)
{
	int startTick = SDL_GetTicks();

	SDL_SetWindowTitle(update.window, to_string(SDL_GetTicks()).append(" MS have passed").c_str());

	SDL_RenderClear(update.renderer);

	window = update.window;


	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->update(update);
	}

	getPlayer();

	SDL_SetRenderDrawColor(update.renderer, 10, 10, 10, 255);

	SDL_RenderPresent(update.renderer);

	int delta = SDL_GetTicks() - startTick;
	gameFPS = 1000 / delta;
}


Player* Game::getPlayer()
{
	static Player* p;
	if (!p)
		p = new Player(400,250);
	return p;
}



void Game::keyDown(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = true;
	for (std::vector<Object*>::iterator i = objects.begin(); i != objects.end(); ++i)
	{
		(*i)->keyDown(ev);
	}
}

void Game::keyUp(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = false;
}

void Game::addGlobalObject(Object* obj)
{
	objects.push_back(obj);
}

void Game::removeGlobalObject(Object* obj)
{
	// TODO (i'm breaddead)

}

bool Game::getKey(int code)
{
	if (controls.count(code) != 1)
		return false;
	return controls[code];
}
