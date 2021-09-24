#include "includes.h"
#include "Game.h"
#include "Object.h"

Object::Object(int x, int y)
{
	std::cout << "Created an object\n";
	Game::addGlobalObject(this);
}
