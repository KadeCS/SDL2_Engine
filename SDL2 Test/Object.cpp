#include "includes.h"
#include "Object.h"

#include "Game.h"
#include "Utils.h"


Object::Object(float x, float y)
{
	touchingBound = false;
	setX(x);
	setY(y);
}

void Object::create()
{
	Game::addGlobalObject(this);
}

void Object::setX(float x)
{
	if (isDead)
		return;
	float xBound = SDL_GetWindowSurface(Game::window)->w - 10;
	float yBound = SDL_GetWindowSurface(Game::window)->h - 10;
	if (x >= xBound || y >= yBound)
		touchingBound = true;
	else
		touchingBound = false;
	this->x = Utils::clamp(x, 0, xBound);
}

void Object::setY(float y)
{
	if (isDead)
		return;
	float xBound = SDL_GetWindowSurface(Game::window)->w - 10;
	float yBound = SDL_GetWindowSurface(Game::window)->h - 10;
	if (x >= xBound || y >= yBound)
		touchingBound = true;
	else
		touchingBound = false;
	this->y = Utils::clamp(y, 0, yBound);
}

bool Object::isColiding(Object* obj, float overrideX, float overrideY)
{
	float leftA, leftB;
	float rightA, rightB;
	float topA, topB;
	float bottomA, bottomB;

	leftB = obj->x;
	rightB = obj->x + obj->w;
	topB = obj->y;
	bottomB = obj->y + obj->h;

	leftA = overrideX;
	rightA = overrideX + this->w;
	topA = overrideY;
	bottomA = overrideY + this->h;

	std::cout << "checking (" << leftB << "," << rightB << ") vs (" << leftA << "," << rightA << ")" << std::endl;

	

	std::cout << "yes i'm coliding" << std::endl;

	return true;
}

void Object::die()
{
	isDead = true;
	Game::removeGlobalObject(this);
	if (!this)
		delete this;
}

