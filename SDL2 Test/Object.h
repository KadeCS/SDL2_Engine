#pragma once
#include "includes.h"
#include "Packets.h"

class Object
{
	public:
		Object(float x, float y);
		Object() = default;
		~Object() = default;
		void create();
		virtual void update(Events::updateEvent ev) = 0;
		virtual void keyDown(SDL_KeyboardEvent ev) {};

		float x = 0;
		float y = 0;
		float w = 0;
		float h = 0;

		bool isLocal = false;

		bool touchingBound;

		bool isDead = false;

		void die();

		bool isColiding(Object* obj);

		int type;


		virtual void setX(float x);
		virtual void setY(float y);
};

