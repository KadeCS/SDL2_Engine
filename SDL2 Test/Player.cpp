#include "includes.h"
#include "Game.h"
#include "Bullet.h"
#include "Assets.h";
#include <cmath>
#include "Utils.h"
#include <iostream>

#define GRAVITY = 0.4

bool pressedM = false;

bool reloading = false;

int reloadingTimer = 0;

float xAcc = 0;

Entity mpEntity;

int jumps = 1;

Asset* getPlayerAsset()
{
	static Asset* as;
	if (!as)
		as = Assets::getAsset("player.png", Game::renderer);

	return as;
}

Player::Player(float x, float y) : Object(x, y)
{
}

void Player::update(Events::updateEvent ev)
{
	if (isDead)
		return;

	isLocal = Multiplayer::localId == mpEntity.id;

	if (isLocal)
	{

		if (reloading)
		{
			if (ammo < 6)
			{
				if (reloadingTimer % 400 == 0)
				{
					ammo++;
				}
			}
			else
			{
				reloading = false;
			}
			reloadingTimer += Game::deltaTime;
		}

		if (ammo > 6)
			ammo = 6;

		if (touchingBound)
			jumps = 1;

		if (Game::getKey(SDLK_a))
		{
			xAcc = -0.2;
		}

		if (Game::getKey(SDLK_d))
		{
			xAcc = 0.2;
		}

		// mouse stuff

		int m_x, m_y;

		Uint32 buttons;

		SDL_PumpEvents();

		buttons = SDL_GetMouseState(&m_x, &m_y);

		if ((buttons & SDL_BUTTON_LMASK) != 0 && !pressedM && ammo > 0) {
			// pressed lmb

			reloading = false;

			float deltaX = m_x - x;
			float deltaY = m_y - y;

			float len = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

			float v1 = deltaX / len;
			float v2 = deltaY / len;

			CPacketShoot_t shoot;

			shoot.Order = 0;
			shoot.PacketType = CPacketShoot;
			vector2 pos;
			vector2 vector;
			vector2 mp;

			pos.x = x;
			pos.y = y;

			vector.x = v1;
			vector.y = v2;

			shoot.Position = pos;

			shoot.VelocityDelta = vector;

			mp.x = m_x;
			mp.y = m_y;

			shoot.MousePosition = mp;

			std::cout << "sent bullet towards " << v1 << "," << v2 << std::endl;

			Multiplayer::sendMessage<CPacketShoot_t>(shoot);

			pressedM = true;
		}



		if ((buttons & SDL_BUTTON_LMASK) == 0 && pressedM) {
			// released lmb

			pressedM = false;
		}

		if (xAcc > 0)
			xAcc -= 0.001;
		if (xAcc < 0)
			xAcc += 0.001;

		if (xAcc < 0.01 && xAcc > -0.01)
			xAcc = 0;

		if (xAcc > 0.6)
			xAcc = 0.6;

		xVel = xAcc;

		if (yVel < 1)
			yVel += 0.001;

		if (yVel > 0.4)
			yVel = 0.4;

		setY(y + yVel);
		setX(x + xVel);
	}

	SDL_FRect rect;

	rect.x = x;
	rect.y = y;
	rect.w = getPlayerAsset()->w;
	rect.h = getPlayerAsset()->h;


	SDL_RenderCopyF(ev.renderer, getPlayerAsset()->texture, NULL, &rect);


	if (isLocal && SDL_GetTicks() % 35 == 0)
	{
		CPacketUpdateEntity_t update;

		update.entityid = mpEntity.id;
		update.PacketType = CPacketUpdateEntity;
		update.Order = 0;

		vector2 vec;

		vec.x = x;
		vec.y = y;

		update.Position = vec;

		Multiplayer::sendMessage<CPacketUpdateEntity_t>(update);
	}
	else if (!isLocal)
	{
		SDL_Surface* screen = SDL_GetWindowSurface(ev.window);
		positionTime += Game::deltaTime / 200;
		setX(std::lerp(lastX, toX, Utils::clamp(positionTime,0,1)));
		setY(std::lerp(lastY, toY, Utils::clamp(positionTime,0,1)));
	}
	if (!username)
	{
		username = new TextDisplay(x - 25, y - 25, mpEntity.username, 50, 10);
		username->create();
	}

	username->setX(x - 25);
	username->setY(y - 25);

	if (!hasStarted)
		hasStarted = true;
}

void Player::keyDown(SDL_KeyboardEvent ev)
{
	if (Game::getKey(SDLK_w) && isLocal && jumps > 0)
	{
		jumps--;
		yVel = -0.3;
	}

	if (Game::getKey(SDLK_r) && isLocal && !reloading)
	{
		reloadingTimer = 0;
		reloading = true;
		std::cout << "reloading";
	}
}

void Player::setX(float x)
{
	if (isDead)
		return;

	// check collision

	if (isLocal)
	{
		for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
		{
			Object* obj = (*Game::getGlobalObjects())[i];
			if (obj->isColiding(this) && (obj->type == Player_e || obj->type == Wall_e))
			{
				touchingBound = true;
				return;
			}
		}
	}

	float xBound = SDL_GetWindowSurface(Game::window)->w - 10;
	float yBound = SDL_GetWindowSurface(Game::window)->h - 10;
	if (x >= xBound || y >= yBound)
		touchingBound = true;
	else
		touchingBound = false;
	this->x = Utils::clamp(x, 0, xBound);
}

void Player::setY(float y)
{
	if (isDead)
		return;

	// check collision

	if (isLocal)
	{
		for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
		{
			Object* obj = (*Game::getGlobalObjects())[i];
			if (obj->isColiding(this) && (obj->type == Player_e || obj->type == Wall_e))
			{
				touchingBound = true;
				return;
			}
		}
	}

	float xBound = SDL_GetWindowSurface(Game::window)->w - 10;
	float yBound = SDL_GetWindowSurface(Game::window)->h - 10;
	if (x >= xBound || y >= yBound)
		touchingBound = true;
	else
		touchingBound = false;
	this->y = Utils::clamp(y, 0, yBound);
}

void Player::onShot(SPacketShootResponse_t ev)
{

	float deltaX = ev.MousePosition.x - x;
	float deltaY = ev.MousePosition.y - y;

	float dist = sqrt(pow(ev.MousePosition.x - x, 2) + pow(ev.MousePosition.y - y, 2));
	float len = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

	if (dist > 0.6)
		dist = 0.6;

	float v1 = deltaX / len;
	float v2 = deltaY / len;


	if (deltaX < 0)
	{
		xAcc = dist;
	}
	else
	{
		xAcc = -dist;
	}

	if (deltaY < 0)
	{
		yVel = dist;
	}
	else
	{
		yVel = -dist;
	}

	ammo--;
}
