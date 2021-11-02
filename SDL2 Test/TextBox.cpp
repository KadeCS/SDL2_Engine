#include "TextBox.h"
#include "includes.h"
#include <SDL_ttf.h>
#include "Utils.h"
#include "Game.h"

void TextBox::update(Events::updateEvent event)
{
	rect.x = x;
	rect.y = y;

	SDL_SetRenderDrawColor(event.renderer, 255, 255, 255, 255);

	SDL_RenderFillRectF(event.renderer, &rect);

	SDL_Surface* screen = SDL_GetWindowSurface(event.window);

	std::string display = *text;

	display.append((isFocused ? "_" : ""));

	SDL_Surface* surfaceMessage =
		TTF_RenderText_Blended_Wrapped(Utils::getFont(), display.c_str(), { color.r, color.g,color.b }, screen->w);

	SDL_Texture* message = SDL_CreateTextureFromSurface(event.renderer, surfaceMessage);

	SDL_Rect message_Rect;

	SDL_QueryTexture(message, NULL, NULL, &rW, &rH);

	rect.w = w;
	rect.h = rH;

	message_Rect.x = x;
	message_Rect.y = y;
	message_Rect.w = rW;
	message_Rect.h = rH;


	int m_x, m_y;

	Uint32 buttons;

	buttons = SDL_GetMouseState(&m_x, &m_y);

	if ((buttons & SDL_BUTTON_LMASK) != 0)
	{
		SDL_Rect colOne;

		colOne.x = m_x;
		colOne.y = m_y;
		colOne.w = 10;
		colOne.h = 10;

		SDL_Rect colTwo;

		colTwo.x = rect.x;
		colTwo.y = rect.y;
		colTwo.w = rect.w;
		colTwo.h = rect.h;

		if (SDL_HasIntersection(&colOne, &colTwo))
			isFocused = true;
		else
			isFocused = false;
	}

	SDL_RenderCopy(event.renderer, message, NULL, &message_Rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void TextBox::keyDown(SDL_KeyboardEvent ev)
{
	if (ev.keysym.sym == SDLK_BACKSPACE && (*text).length() > 0)
	{
		text->pop_back();
	}
}

void TextBox::textInput(SDL_TextInputEvent ev)
{
	if (isFocused && (*text).length() < 6)
	{
		text->append(ev.text);
	}
}



void TextBox::setText(std::string _text)
{
	std::string* t = new std::string(_text);

	delete text;

	this->text = t;

}
