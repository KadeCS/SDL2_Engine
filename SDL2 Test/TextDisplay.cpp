#include "TextDisplay.h"
#include "Utils.h"
#include "Utils.h"
#include "Game.h"

void TextDisplay::setText(std::string temp)
{

	std::string* _text = new std::string(temp);

	delete text;

	this->text = _text;

	SDL_Surface* screen = SDL_GetWindowSurface(Game::window);

	if (surfaceMessage != nullptr)
	{
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(message);
	}

	surfaceMessage =
		TTF_RenderText_Blended_Wrapped(Utils::getFont(), text->c_str(), { color.r, color.g,color.b }, screen->w);
	message = SDL_CreateTextureFromSurface(Game::renderer, surfaceMessage);
}

void TextDisplay::update(Events::updateEvent ev)
{
	SDL_QueryTexture(message, NULL, NULL, &rW, &rH);

	message_Rect.x = x;
	message_Rect.y = y;
	message_Rect.w = rW;
	message_Rect.h = rH;

	SDL_SetRenderTarget(ev.renderer, NULL);

	SDL_RenderCopy(ev.renderer, message, NULL, &message_Rect);
}
