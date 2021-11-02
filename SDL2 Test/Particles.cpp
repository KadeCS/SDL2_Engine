#include "Particles.h"


int time = 0;
int vector1 = 0;
int vector2 = 0;

struct Particle
{
	SDL_FRect rect;
	int alpha = 255;
};

std::vector<Particle>* particleRects;

Particles::Particles(int v1, int v2, int n, int msTime)
{
	particleRects = new std::vector<Particle>();
	for (int i = 0; i < n; i++)
	{
		Particle p;
		p.rect.x = x;
		p.rect.y = y;
		int blah = 4 + rand() % ((10 + 4) - 4);
		p.rect.w = blah;
		p.rect.h = blah;
		particleRects->push_back(p);
	}
}

void Particles::update(Events::updateEvent event)
{
	// TODO:
	// create particles and make em go across a vector with random deviation
}
