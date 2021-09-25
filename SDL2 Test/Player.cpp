#include "includes.h"
#include "Player.h"
#include "Game.h"

#include <SDL_ttf.h>


void Player::update(Events::updateEvent ev)
{

	if (Game::getKey(SDLK_w) && SDL_GetTicks() % 8 == 0)
	{
		setY(this->y - 1);
	}

	if (Game::getKey(SDLK_a) && SDL_GetTicks() % 8 == 0)
	{
		setX(this->x - 1);
	}

	if (Game::getKey(SDLK_d) && SDL_GetTicks() % 8 == 0)
	{
		setX(this->x + 1);
	}

	if (Game::getKey(SDLK_s) && SDL_GetTicks() % 8 == 0)
	{
		setY(this->y + 1);
	}

	SDL_Rect rect;

	rect.x = x;
	rect.y = y;
	rect.w = 10;
	rect.h = 10;

	SDL_SetRenderDrawColor(ev.renderer, 255, 255, 255, 255);

	SDL_RenderDrawRect(ev.renderer, &rect);


	// Render hud and other stuff

	TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Solid(Sans, ("FPS: " + std::to_string(Game::gameFPS)).c_str(), { 255,255,255 });

	SDL_Texture* message = SDL_CreateTextureFromSurface(ev.renderer, surfaceMessage);

	SDL_Rect message_Rect;

	message_Rect.x = 0;
	message_Rect.y = 0;
	message_Rect.w = 100;
	message_Rect.h = 100;

	SDL_RenderCopy(ev.renderer, message, NULL, &message_Rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}
