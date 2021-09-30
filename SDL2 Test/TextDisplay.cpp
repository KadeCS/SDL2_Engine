#include "TextDisplay.h"
#include "Utils.h"
#include "Game.h"

void TextDisplay::setText(std::string temp)
{

	std::string* _text = new std::string(temp);

	delete text;

	this->text = _text;

	TTF_SizeText(Utils::getFont(), text->c_str(), &rW, &rH);

	rW = rW + w;
	rH = rH + h;
}

void TextDisplay::update(Events::updateEvent ev)
{
	SDL_Surface* screen = SDL_GetWindowSurface(ev.window);

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Blended_Wrapped(Utils::getFont(), text->c_str(), { 255,255,255 },screen->w);

	SDL_Texture* message = SDL_CreateTextureFromSurface(ev.renderer, surfaceMessage);

	SDL_Rect message_Rect;

	message_Rect.x = x;
	message_Rect.y = y;
	message_Rect.w = rW;
	message_Rect.h = rH;

	SDL_RenderCopy(ev.renderer, message, NULL, &message_Rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}
