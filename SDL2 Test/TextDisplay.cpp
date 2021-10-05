#include "TextDisplay.h"
#include "Utils.h"
#include "Game.h"

void TextDisplay::setText(std::string temp)
{

	std::string* _text = new std::string(temp);

	delete text;

	this->text = _text;
}

void TextDisplay::update(Events::updateEvent ev)
{
	SDL_Surface* screen = SDL_GetWindowSurface(ev.window);

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Blended_Wrapped(Utils::getFont(), text->c_str(), { color.r, color.g,color.b },screen->w);

	SDL_Texture* message = SDL_CreateTextureFromSurface(ev.renderer, surfaceMessage);

	SDL_Rect message_Rect;

	SDL_QueryTexture(message, NULL, NULL, &rW, &rH);

	message_Rect.x = x;
	message_Rect.y = y;
	message_Rect.w = rW;
	message_Rect.h = rH;

	SDL_SetRenderTarget(ev.renderer, NULL);

	SDL_RenderCopy(ev.renderer, message, NULL, &message_Rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}
