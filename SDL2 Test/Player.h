#pragma once
#include "Object.h"
#include "Multiplayer.h"
#include "Packets.h"
#include "TextDisplay.h"
class Player : public Object
{
	public:
		Player(float x, float y);
		Player() = default;
		~Player() = default;
		void update(Events::updateEvent event) override;
		void keyDown(SDL_KeyboardEvent ev) override;

		void setX(float x) override;
		void setY(float y) override;

		void onShot(SPacketShootResponse_t ev);
		float yVel = 0;
		float xVel = 0;
		float xAcc = 0;

		float r = 255;
		float g = 255;
		float b = 255;

		float localHitTime = 0;
		bool hit = false;

		float health = 100;

		int ammo = 6;

		bool waitingOnShot = false;

		SDL_FRect rect;

		void checkCol();

		float lastX = 0;
		float lastY = 0;
		float toX = 0;
		float toY = 0;

		int w = 10;
		int h = 10;

		bool hasStarted = false;

		bool touchingPlayer;

		float positionTime = 0;

		Entity mpEntity;

		TextDisplay* username;
};

