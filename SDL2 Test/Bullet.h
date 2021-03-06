#pragma once
#include "Object.h"
#include "Packets.h"
#include <vector>
class Bullet : public Object
{
	public:
		Bullet(float x, float y) : Object(x, y) {
		
			type = Bullet_e;
		};
		Bullet() = default;
		~Bullet() = default;
		void update(Events::updateEvent event) override;

		int type = Bullet_e;

		SDL_FRect rect;

		Entity mpEntity;

		vector2 direction;

		float bulletX = 0;
		float bulletY = 0;


		static void addBullet(Bullet* b);
		static void removeBullet(Bullet* b);
		static std::vector<Bullet*>* getBullets();

};

