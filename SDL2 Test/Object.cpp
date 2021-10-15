#include "includes.h"
#include "Object.h"

#include "Game.h"
#include "Utils.h"
#include "Wall.h"


Object::Object(float x, float y)
{
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
	this->x = Utils::clamp(x, 0, xBound);
}

void Object::setY(float y)
{
	if (isDead)
		return;
	float yBound = SDL_GetWindowSurface(Game::window)->w - 10;
	this->y = Utils::clamp(y, 0, yBound);
}

void Object::die()
{
	isDead = true;
	if (isCreated)
		Game::removeGlobalObject(this);
	if (!this)
		delete this;
}

bool Object::isColidingX(Object* obj, int pointX)
{
	Player* p = (Player*)this;
	Player* pp;
	Wall* wall;

	switch (obj->type)
	{
		case Player_e:
			pp = (Player*)obj;
			if ((pp->x <= p->x + p->rect.w && pp->x + pp->rect.w >= pointX))
				return true;
			break;
		case Wall_e:
			wall = (Wall*)obj;
			if ((wall->x <= p->x + p->rect.w && wall->x + wall->rect.w >= pointX))
				return true;
			break;
	}

	return false;
}

bool Object::isColidingY(Object* obj, int pointY)
{
	Player* p = (Player*)this;
	Player* pp;
	Wall* wall;

	switch (obj->type)
	{
	case Player_e:
		pp = (Player*)obj;
		if ((pp->y <= p->y + p->rect.h && pp->y + pp->rect.h >= pointY))
			return true;
		break;
	case Wall_e:
		wall = (Wall*)obj;
		if ((wall->y <= p->y + p->rect.h && wall->y + wall->rect.h >= pointY))
			return true;
		break;
	}

	return false;
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