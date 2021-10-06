#pragma once
#include "Object.h"
#include "includes.h"
#include "Packets.h"

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class TextDisplay : public Object
{
	public:

		TextDisplay(int x, int y, std::string temp, int w, int h) : Object(x, y) {

			std::string* _text = new std::string(temp);

			this->text = _text;
			this->w = w;
			this->h = h;
			this->rW = w;
			this->rH = h;
			this->type = Misc_e;

			color = { 255,255,255 };

			setText(temp);
		};


		TextDisplay() = default;
		~TextDisplay() = default;
		void update(Events::updateEvent event) override;

		Color color;

		SDL_Surface* surfaceMessage;
		SDL_Texture* message;
		SDL_Rect message_Rect;

		std::string* text;
		int w;
		int h;

		int rW;
		int rH;

		void setText(std::string _text);
};

