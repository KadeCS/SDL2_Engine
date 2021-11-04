#include "Particles.h"
#include "Utils.h"




Particles::Particles(float x, float y, float v1, float v2, int n, int msTime)
{
	for (int i = 0; i < n; i++)
	{
		Particle p;
		p.rect.x = x;
		p.rect.y = y;
		int blah = 4 + rand() % ((10 + 4) - 4);
		p.rect.w = blah;
		p.rect.h = blah;
		particleRects.push_back(p);
		p.devianceX = 2 + rand() % ((6 + 2) - 2);
		p.devianceY = 8 + rand() % ((12 + 8) - 8);
	}
	duration = msTime;
	vector1 = v1;
	vector2 = v2;
	startTime = SDL_GetTicks();
	std::cout << startTime + duration << " | " << SDL_GetTicks() << " <" << v1 << "," << v2 << ">" << std::endl;
}


void Particles::update(Events::updateEvent event)
{
	for (int i = 0; i < particleRects.size(); i++)
	{
		Particle p = particleRects[i];

		SDL_FRect newRect;

		newRect.w = Utils::lerp(p.rect.w, 0, bruh);
		newRect.h = Utils::lerp(p.rect.h, 0, bruh);

		newRect.x = Utils::lerp(p.rect.x, p.rect.x + (vector1 + p.devianceX) * 20, bruh);
		newRect.y = Utils::lerp(p.rect.y, p.rect.y + (vector2 + p.devianceY) * 20, bruh);

		p.rect = newRect;
		p.alpha = Utils::lerp(p.alpha, 0, bruh);

		SDL_SetRenderDrawColor(event.renderer,255,255,255,p.alpha);
		SDL_RenderFillRectF(event.renderer, &p.rect);
	}

	if (particleRects.size() == 0 || bruh > 1)
	{
		die();
	}

	bruh = (SDL_GetTicks() - startTime) / duration;

}
