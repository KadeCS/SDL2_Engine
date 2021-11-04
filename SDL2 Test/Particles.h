#pragma once
#include "Object.h"
struct Particle
{
	SDL_FRect rect;
	int alpha = 255;
	double angle;
	float devianceX;
	float devianceY;
};

class Particles : public Object
{
public:
	Particles(float x, float y, float v1, float v2, int n, int msTime);
	~Particles() = default;

	std::vector<Particle> particleRects;
	float duration = 0;
	float startTime = 0;
	float vector1 = 0;
	float vector2 = 0;
	double bruh = 0;


	void update(Events::updateEvent event) override;

};

