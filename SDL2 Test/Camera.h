#pragma once
#include "Object.h"

class Camera : public Object
{
	public:
		Camera();
		~Camera() = default;

		SDL_Texture* cameraTexture;

		void update(Events::updateEvent event) override;

		void shakeEffect(int intensity, int ms);

		int w = 792;
		int h = 600;
};

