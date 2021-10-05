#pragma once
#include "Object.h"
#include "includes.h"
#include "TextDisplay.h"
class TextBox : public Object
{
public:

	TextBox(int x, int y, std::string temp, int w, int h) : Object(x, y) {

		std::string* _text = new std::string(temp);

		this->text = _text;
		this->w = w;
		this->h = h;
		this->rW = w;
		this->rH = h;
		this->type = Misc_e;

		isFocused = false;
	};
	TextBox() = default;
	~TextBox() = default;
	void update(Events::updateEvent event) override;

	void keyDown(SDL_KeyboardEvent ev) override;

	SDL_FRect rect;

	bool isFocused;

	void textInput(SDL_TextInputEvent ev) override;

	std::string* text;

	Color color;
	int w;
	int h;

	int rW;
	int rH;

	void setText(std::string _text);
};

