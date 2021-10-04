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
		virtual int* getW(int* defaultW);
		virtual int* getH(int* defaultH);

		bool isLocal = false;

		bool touchingBoundX;
		bool touchingBoundY;

		bool isDead = false;

		void die();

		bool isColiding(Object* obj, Object* self);

		int type;


		virtual void setX(float x);
		virtual void setY(float y);
};

