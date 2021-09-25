#pragma once
class Object
{
	public:
		Object(int x, int y);
		Object() = default;
		~Object() = default;
		virtual void update(Events::updateEvent ev) = 0;
		virtual void keyDown(SDL_KeyboardEvent ev) {};

		int x;
		int y;

		void setX(int x);
		void setY(int y);
};

