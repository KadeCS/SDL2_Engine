#include "includes.h"
#include "Object.h"

#include "Game.h"
#include "Utils.h"
#include "Wall.h"


Object::Object(float x, float y)
{
	touchingBoundNegX = false;
	touchingBoundNegY = false;
	touchingBoundX = false;
	touchingBoundY = false;
	setX(x);
	setY(y);
	isCreated = false;
}

void Object::create()
{
	Game::addGlobalObject(this);
	isCreated = true;
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
		recS.x += 1;
		recS.y += 1;
		if (SDL_HasIntersection(&rec, &recS))
			return true;
	}
	else if (obj->type == Wall_e)
	{
		Player* pSelf = (Player*)self;
		Wall* w = (Wall*)obj;

		SDL_Rect rec;

		rec.x = (int)w->rect.x;
		rec.y = (int)w->rect.y;
		rec.w = w->rect.w;
		rec.h = w->rect.h;

		SDL_Rect recS;

		recS.x = (int)pSelf->rect.x;
		recS.y = (int)pSelf->rect.y;
		recS.w = pSelf->rect.w;
		recS.h = pSelf->rect.h;

		if (SDL_HasIntersection(&rec, &recS))
			return true;
		recS.x += 1;
		if (SDL_HasIntersection(&rec, &recS))
		{
			self->touchingBoundX = true;
		}
		recS.y += 1;
		if (SDL_HasIntersection(&rec, &recS))
		{
			self->touchingBoundY = true;
		}
	}

	return false;
}

void Object::die()
{
	isDead = true;
	if (isCreated)
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