#include "Bullet.h"
#include "Game.h"
#include "Utils.h"

void Bullet::update(Events::updateEvent ev)
{
	if (isDead)
		return;

	if (SDL_GetTicks() % 2 == 0)
	{
		bulletX += direction.x * 20;
		bulletY += direction.y * 20;
	}

	rect.x = bulletX;
	rect.y = bulletY;
	rect.w = 4;
	rect.h = 4;

	SDL_SetRenderDrawColor(ev.renderer, 255, 255, 255, 255);

	SDL_RenderDrawRectF(ev.renderer, &rect);

}

std::vector<Bullet*>* Bullet::getBullets()
{
	static std::vector<Bullet*>* bullets;
	if (!bullets)
		bullets = new std::vector<Bullet*>();
	return bullets;
}

void Bullet::addBullet(Bullet* b)
{
	getBullets()->push_back(b);
}

void Bullet::removeBullet(Bullet* b)
{
	getBullets()->erase(std::remove(getBullets()->begin(), getBullets()->end(), b), getBullets()->end());
}
