#include "Camera.h"

bool shaking = false;
int shakeIntensity = 0;
int shakeTimer = 0;



Camera::Camera()
{
	std::srand(std::time(nullptr));
}

void Camera::update(Events::updateEvent event)
{
	if (shaking)
	{
		int min = -8 * shakeIntensity;
		int max = 8 * shakeIntensity;
		x = min + rand() % ((max + min) - min);
		y = min + rand() % ((max + min) - min);


		if (SDL_GetTicks() > shakeTimer)
		{
			shaking = false;
			shakeIntensity = 0;
			x = 0;
			y = 0;
		}
	}
}

void Camera::shakeEffect(int intensity, int ms)
{
	shakeTimer = SDL_GetTicks() + ms;
	shakeIntensity = intensity;
	shaking = true;
}
