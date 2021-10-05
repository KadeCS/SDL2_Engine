#include "includes.h"
#include "Game.h"
#include "Player.h"
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

	Asset* as = getPlayerAsset();
	


	isLocal = Multiplayer::localId == mpEntity.id;

	if (isLocal && SDL_GetTicks() % 2 == 0)
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

		if (touchingBoundY)
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

		xVel = xAcc;

		if (yVel < 1)
			yVel += 0.001;

		if (yVel > 0.4)
			yVel = 0.4;


		setY(y + yVel);
		setX(x + xVel);

		if (touchingBoundY)
			yVel = 0;

		xAcc = 0;
	}

	rect.x = x;
	rect.y = y;
	rect.w = *getW(&as->w);
	rect.h = *getH(&as->h);

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
		setX(Utils::lerp(lastX, toX, Utils::clamp(positionTime, 0, 1)));
		setY(Utils::lerp(lastY, toY, Utils::clamp(positionTime, 0, 1)));
	}
	if (!username)
	{
		username = new TextDisplay(x + 10, y - 35, mpEntity.username, 25, 25);
		username->create();
	}

	username->setX(x - (username->rW / 4));
	username->setY(y - 35);

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

bool checkCol(Player* self, int newX, int newY)
{
	for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
	{
		Object* obj = (*Game::getGlobalObjects())[i];
		Player* p = NULL;
		switch (obj->type)
		{
			case Player_e:
				p = (Player*)obj;
				if (p->mpEntity.id != self->mpEntity.id)
					if (self->isColiding(p, self))
					{
						self->touchingBoundY = true;
						return true;
					}
				break;
			case Wall_e:
				if (self->isColiding(obj, self))
				{
					self->touchingBoundY = true;
					return true;
				}
				break;
		}
	}
	return false;
}

void Player::setX(float nx)
{
	if (isDead)
		return;

	// check collision

	float previousX = x;

	int* w = getW(0);

	if (w == nullptr)
		return;

	float xBound = SDL_GetWindowSurface(Game::window)->w - *w;
	if (nx >= xBound)
		touchingBoundX = true;
	else
		touchingBoundX = false;
	this->x = Utils::clamp(nx, 0, xBound);

	rect.x = x;

	if (isLocal)
		if (checkCol(this, nx, this->y))
		{
			this->x = previousX;
			touchingBoundX = true;
			rect.x = x;
			return;
		}

}

void Player::setY(float ny)
{
	if (isDead)
		return;

	// check collision

	float previousY = y;

	int* h = getH(0);

	if (h == nullptr)
		return;

	float yBound = SDL_GetWindowSurface(Game::window)->h - *h;
	if (ny >= yBound)
		touchingBoundY = true;
	else
		touchingBoundY = false;
	this->y = Utils::clamp(ny, 0, yBound);

	rect.y = y;

	if (isLocal)
		if (checkCol(this, ny, this->y))
		{
			this->y = previousY;
			touchingBoundY = true;
			rect.y = y;
			return;
		}
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
