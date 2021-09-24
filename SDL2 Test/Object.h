#pragma once
class Object
{
	public:
		Object(int x, int y);
		Object() = default;
		~Object() = default;
		virtual void update(Events::updateEvent ev) = 0;
	
};

