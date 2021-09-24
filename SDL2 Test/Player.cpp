#include "includes.h"
#include "Player.h"
#include "Game.h"
Player::Player(int x, int y)
{
	std::cout << "created a player!";
	Game::addGlobalObject(this);
}


void Player::update(Events::updateEvent ev)
{
	std::cout << "update\n";
	Game::removeGlobalObject(this);
}