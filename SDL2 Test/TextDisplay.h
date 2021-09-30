#pragma once
#include "Object.h"
#include "includes.h"
#include "Packets.h"
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
		};
		TextDisplay() = default;
		~TextDisplay() = default;
		void update(Events::updateEvent event) override;

		std::string* text;
		int w;
		int h;

		int rW;
		int rH;

		void setText(std::string _text);
};

