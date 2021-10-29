#include "includes.h"
#include <algorithm>
#include "Multiplayer.h"

#include "Game.h"
#include "Bullet.h"
#include "Player.h"
#include "MainMenu.h"

using namespace std;

vector<Object*>* objects;

Menu* Game::currentMenu = NULL;

map<int, bool> Game::controls = {
	{SDLK_w, false},
	{SDLK_s, false},
	{SDLK_a, false},
	{SDLK_d, false},
	{SDLK_r, false}
};

SDL_Renderer* Game::renderer;
SDL_Window* Game::window;

double Game::gameFPS = 0.0;

float Game::deltaTime = 0;

float Game::startTick = 0;


void Game::createGame()
{
	objects = new std::vector<Object*>();

	// to start
	currentMenu = new MainMenu();
}

bool Game::containsObject(unsigned int id)
{
	for (int i = 0; i < objects->size(); i++)
	{
		Object* obj = (*objects)[i];

		if (obj->type != Player_e)
			continue;

		Player* p = (Player*)obj;

		if (!p)
			continue;
		if (p->mpEntity.id == id)
			return true;
	}
	return false;
}

void Game::update(Events::updateEvent update)
{
	SDL_RenderClear(update.renderer);

	currentMenu->update(update);

	for (int i = 0; i < objects->size(); i++)
	{
		try
		{
			Object* bruh = (*objects)[i];
			bruh->update(update);
		}
		catch (...)
		{

		}
	}

}

void Game::keyDown(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = true;

	currentMenu->keyDown(ev);

	for (int i = 0; i < objects->size(); i++)
	{
		Object* bruh = (*objects)[i];
		bruh->keyDown(ev);
	}

}

void Game::keyUp(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = false;

	currentMenu->keyUp(ev);
}

std::vector<Object*>* Game::getGlobalObjects()
{
	return objects;
}

void Game::addGlobalObject(Object* obj)
{
	objects->push_back(obj);
}

void Game::removeGlobalObject(Object* obj)
{
	objects->erase(std::remove(objects->begin(), objects->end(), obj), objects->end());
}

bool Game::getKey(int code)
{
	if (controls.count(code) != 1)
		return false;
	return controls[code];
}

void Game::textInput(SDL_TextInputEvent event)
{
	for (int i = 0; i < objects->size(); i++)
	{
		Object* fuck = (*objects)[i];
		fuck->textInput(event);
	}
}