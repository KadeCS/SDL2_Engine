#include "includes.h"
#include "Game.h"
#include "Player.h"
#include "Bullet.h"
#include "Assets.h"
#include "Wall.h"
#include <cmath>
#include "Utils.h"
#include "Particles.h"
#include <iostream>

#include <math.h>

#define GRAVITY = 0.4

bool pressedM = false;

bool reloading = false;

int reloadingTimer = 0;

float xAcc = 0;

bool shot = false;

Entity mpEntity;

int framesJumped = 0;
int jumps = 1;

void Player::checkCol()
{
	for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
	{
		Object* obj = (*Game::getGlobalObjects())[i];

		SDL_Rect r;
		SDL_Rect objR;
		switch (obj->type)
		{
			case Wall_e:
				r.x = rect.x + xVel;
				r.y = rect.y + yVel;
				r.w = rect.w + xVel;
				r.h = rect.h + yVel;
				objR.x = ((Wall*)obj)->rect.x;
				objR.y = ((Wall*)obj)->rect.y;
				objR.w = ((Wall*)obj)->rect.w;
				objR.h = ((Wall*)obj)->rect.h;


				r.x += 1;
				if (SDL_HasIntersection(&r, &objR))
				{
					xVel = 0;
				}
				r.x -= 2;
				if (SDL_HasIntersection(&r, &objR))
				{
					xVel = 0;
				}
				r.x = rect.x;
				r.y += 1;
				if (SDL_HasIntersection(&r, &objR))
				{
					jumps = 1;
					yVel = 0;
				}
				r.y -= 2;
				if (SDL_HasIntersection(&r, &objR))
				{
					jumps = 0;
					yVel = 0;
				}
				break;
		}
	}
}



Asset* getPlayerAsset()
{
	static Asset* as;
	if (!as)
		as = Assets::getAsset("player.png", Game::renderer);
	return as;
}

Player::Player(float x, float y) : Object(x, y)
{
	type = Player_e;
	touchingPlayer = false;
}

bool reverseAnim = false;

float redFlash = 0;

void Player::update(Events::updateEvent ev)
{
	if (isDead)
		return;

	Asset* as = getPlayerAsset();

	isLocal = Multiplayer::localId == mpEntity.id;

	if (health < 0 || health == 0)
	{
		health = 100;
		setX(396);
		setY(300);
	}

	for (int i = 0; i < Bullet::getBullets()->size(); i++)
	{
		Bullet* b = (*Bullet::getBullets())[i];
		if (!b)
		{
			delete b;
			continue;
		}

		SDL_Rect r;
		SDL_Rect objR;

		r.x = rect.x;
		r.y = rect.y;
		r.w = rect.w;
		r.h = rect.h;
		objR.x = b->rect.x - 12;
		objR.y = b->rect.y - 12;
		objR.w = 24;
		objR.h = 24;

		if (isLocal)
			SDL_RenderDrawRect(Game::renderer, &objR);

		if (SDL_HasIntersection(&r, &objR))
		{
			if (!hit && (!b->isLocal || !isLocal))
			{
				std::cout << "youch!" << std::endl;
				hit = true;
				if (isLocal)
					health -= 25;
				localHitTime = SDL_GetTicks();
			}
		}
	}

	if (hit)
	{
		if (SDL_GetTicks() % 2 == 0)
		{
			if (!reverseAnim)
			{
				alpha = 125;
				reverseAnim = true;
			}
			else if (reverseAnim)
			{
				alpha = 255;
				reverseAnim = false;
			}
		}
		redFlash += Game::deltaTime / 25;
		g = Utils::lerp(g, 125, redFlash);
		b = Utils::lerp(b, 125, redFlash);

		if (localHitTime + 2500 < SDL_GetTicks())
		{
			hit = false;
			alpha = 255;
			g = 255;
			b = 255;
		}
	}

	if (isLocal)
	{

		if (reloading)
		{
			if (ammo < 6)
			{
				if (reloadingTimer % 40 == 0)
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
		else if (ammo < 0)
			ammo = 0;


		if (Game::getKey(SDLK_a))
			xAcc = -2;
		if (Game::getKey(SDLK_d))
			xAcc = 2;

		// mouse stuff

		int m_x, m_y;

		Uint32 buttons;

		buttons = SDL_GetMouseState(&m_x, &m_y);

		if ((buttons & SDL_BUTTON_LMASK) != 0 && !pressedM && ammo > 0 && !waitingOnShot) {
			// pressed lmb


			reloading = false;

			float deltaX = m_x - x;
			float deltaY = m_y - y;

			float len = sqrt(pow(deltaX, 2) + pow(deltaY, 2));

			waitingOnShot = true;

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



		float time = ((SDL_GetTicks() - Game::startTick));

		yVel += 0.1;

		checkCol();

		setY((y + yVel));
		setX((x + xVel));


		if (yVel > 4)
			yVel = 4;
		

		if (xAcc > 0.1)
			xAcc -= 0.2;
		else if (xAcc < -0.1)
			xAcc += 0.2;

		double rounded = std::ceil(xAcc * 100.0) / 100.0;

		if (abs(rounded) <= 0.1 )
			xAcc = 0;
	}

	rect.x = x;
	rect.y = y;
	rect.w = *getW(&as->w);
	rect.h = *getH(&as->h);

	SDL_Texture* texture = getPlayerAsset()->texture;

	SDL_SetRenderDrawBlendMode(ev.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureColorMod(texture, r, g, b);
	SDL_SetTextureAlphaMod(texture, alpha);

	SDL_RenderCopyF(ev.renderer, texture, NULL, &rect);


	if (isLocal && SDL_GetTicks() % 1 == 0)
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
		positionTime += Game::deltaTime / 25;
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
		std::cout << jumps << std::endl;
		yVel = -4;
		setY((y + yVel));
		framesJumped = 0;
	}

	if (Game::getKey(SDLK_r) && isLocal && !reloading)
	{
		reloadingTimer = 0;
		reloading = true;
		std::cout << "reloading";
	}
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
	this->x = Utils::clamp(nx, 0, xBound);

	rect.x = x;
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
	this->y = Utils::clamp(ny, 0, yBound);

	rect.y = y;

}

void Player::onShot(SPacketShootResponse_t ev)
{

	waitingOnShot = false;
	float deltaX = ev.MousePosition.x - x;
	float deltaY = ev.MousePosition.y - y;

	float dist = sqrt(pow(ev.MousePosition.x - x, 2) + pow(ev.MousePosition.y - y, 2));
	float len = sqrt(pow(deltaX, 2) + pow(deltaY, 2));


	if (dist > 1)
		dist = 1;

	float angle = std::atan2(y - ev.MousePosition.y, x - ev.MousePosition.x) * 180 / M_PI;

	deltaX = Utils::clamp(deltaX,-4,4);
	deltaY = Utils::clamp(deltaY,-4,4);

	deltaX = -deltaX;
	deltaY = -deltaY;

	xAcc = deltaX;
	yVel = deltaY;

	std::cout << deltaX << "," << xAcc << "," << yVel << ", LEN " << len << ", ANGLE: " << angle << std::endl;

	Game::mainCamera->shakeEffect(0.8, 100);

	ammo--;
}
