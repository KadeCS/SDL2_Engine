#include "Wall.h"

void Wall::update(Events::updateEvent event)
{
	rect.x = x;
	rect.y = y;
	rect.w = 24;
	rect.h = 24;


	SDL_SetRenderDrawColor(event.renderer, 255, 255, 255, 255);


	SDL_RenderDrawRectF(event.renderer, &rect);
}

