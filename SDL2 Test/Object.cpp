#include "includes.h"
#include "Object.h"

#include "Game.h"
#include "Utils.h"


Object::Object(float x, float y)
{
	touchingBoundX = false;
	touchingBoundY = false;
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
	if (x >= xBound)
		touchingBoundX = true;
	else
		touchingBoundX = false;
	this->x = Utils::clamp(x, 0, xBound);
}

void Object::setY(float y)
{
	if (isDead)
		return;
	float yBound = SDL_GetWindowSurface(Game::window)->w - 10;
	if (y >= yBound)
		touchingBoundY = true;
	else
		touchingBoundY = false;
	this->y = Utils::clamp(y, 0, yBound);
}

bool Object::isColiding(Object* obj, Object* self)
{
	if (obj->type == Player_e)
	{
		Player* p = (Player*)obj;
		Player* pSelf = (Player*)self;

		SDL_Rect rec;

		rec.x = (int) p->rect.x;
		rec.y = (int) p->rect.y;
		rec.w = p->rect.w;
		rec.h = p->rect.h;

		SDL_Rect recS;

		recS.x = (int)pSelf->rect.x;
		recS.y = (int)pSelf->rect.y;
		recS.w = pSelf->rect.w;
		recS.h = pSelf->rect.h;

		if (SDL_HasIntersection(&rec,&recS))
			return true;
	}

	return false;
}

void Object::die()
{
	isDead = true;
	Game::removeGlobalObject(this);
	if (!this)
		delete this;
}

int* Object::getW(int* defaultW)
{
	static int* w;
	if (!w)
		w = defaultW;
	return w;
}

int* Object::getH(int* defaultH)
{
	static int* h;
	if (!h)
		h = defaultH;
	return h;
}