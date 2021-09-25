#pragma once
#include "Object.h"
class Player :
	virtual public Object
{
	public:
		Player(int x, int y) : Object(x, y) {};
		Player() = default;
		~Player() = default;
		void update(Events::updateEvent event) override;

};

