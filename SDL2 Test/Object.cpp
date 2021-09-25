#include "includes.h"
#include "Game.h"
#include "Object.h"
#include "Utils.h"

Object::Object(int x, int y)
{
	Game::addGlobalObject(this);

	setX(x);
	setY(y);
}

void Object::setX(int x)
{
	this->x = Utils::clamp(x, 0, SDL_GetWindowSurface(Game::window)->w - 10);
}

void Object::setY(int y)
{
	this->y = Utils::clamp(y, 0, SDL_GetWindowSurface(Game::window)->h - 10);
}

