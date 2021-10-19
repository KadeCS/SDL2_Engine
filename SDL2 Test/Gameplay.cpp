#include "Gameplay.h"
#include "fmt/format.h"
#include <boost/lexical_cast.hpp>
std::vector<Player*>* entitiesToRemove;
std::vector<Bullet*>* bulletsToRemove;

void Gameplay::reloadLevel()
{
	for (int i = 0; i < currentLevel->savedObjects->size(); i++)
	{
		Wall* w = (Wall*)(*currentLevel->savedObjects)[i];
		w->create();
	}
}

Gameplay::Gameplay()
{
	entitiesToRemove = new std::vector<Player*>();
	bulletsToRemove = new std::vector<Bullet*>();

	currentLevel = Level::LoadLevel("lvl.level");
	reloadLevel();
}

bool containsEntity(unsigned int id)
{
	for (int i = 0; i < Multiplayer::getEntites().size(); i++)
	{
		Entity p = Multiplayer::getEntites()[i];

		if (p.id == id)
			return true;
	}
	return false;
}



void updatePlayers()
{
	for (int i = 0; i < Multiplayer::getEntites().size(); i++)
	{
		Entity en = Multiplayer::getEntites()[i];

		Player* p = nullptr;
		Bullet* b = nullptr;

		switch (en.EntityType)
		{
		case 0:
			if (en.id == Multiplayer::localId)
			{
				continue;
			}

			p = Gameplay::createPlayer(en);
			p->positionTime = 0;
			p->lastX = p->x;
			p->lastY = p->y;
			p->toY = en.position.y;
			p->toX = en.position.x;
			break;
		case 1:
			// update created bullets lol

			bool updated = false;

			for (int i = 0; i < Bullet::getBullets()->size(); i++)
			{
				b = (*Bullet::getBullets())[i];
				if (!b)
				{
					delete b;
					continue;
				}

				if (en.id != b->mpEntity.id)
					continue;

				updated = true;

				b->direction = en.direction;

			}

			if (!updated)
			{
				b = new Bullet(en.position.x, en.position.y);
				b->mpEntity = en;
				Bullet::addBullet(b);
				b->create();
				b->direction = en.direction;
				b->bulletX = en.position.x;
				b->bulletY = en.position.y;
			}



			break;
		}

		if (p == nullptr)
			delete p;
		if (b == nullptr)
			delete b;
	}

	for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
	{
		Object* obj = (*Game::getGlobalObjects())[i];

		Player* p = nullptr;

		switch (obj->type)
		{
		case Player_e:
			p = (Player*)obj;
			if (!p)
				continue;
			if (!p->hasStarted)
				continue;


			if (!containsEntity(p->mpEntity.id))
			{
				entitiesToRemove->push_back(p);
			}
			break;
		}
		if (p == nullptr)
			delete p;
	}

	for (int i = 0; i < Bullet::getBullets()->size(); i++)
	{
		Bullet* b = (*Bullet::getBullets())[i];
		if (!b)
		{
			delete b;
			continue;
		}

		if (!containsEntity(b->mpEntity.id))
		{
			bulletsToRemove->push_back(b);
		}
	}
}

void Gameplay::onLoggedIn()
{
	getLocalPlayer();
	updatePlayers();
}

void Gameplay::onUpdateGameState()
{
	if (Multiplayer::loggedIn)
		updatePlayers();
}



TextDisplay* Gameplay::getHud()
{
	static TextDisplay* hud = nullptr;
	if (!hud)
	{
		hud = new TextDisplay(0, 0, "hello", 100, 100);
		hud->create();
	}
	return hud;

}


Player* Gameplay::getLocalPlayer()
{
	if (!Multiplayer::loggedIn)
		return NULL;
	Player* p = findPlayerById(Multiplayer::localId);
	if (!p)
	{
		Entity en = findEntityById(Multiplayer::localId);

		p = new Player(en.position.x, en.position.y);
		p->mpEntity = en;
		p->isLocal = true;
		p->create();

		std::cout << "created player" << std::endl;
	}
	return p;
}

Entity Gameplay::findEntityById(unsigned int id)
{
	if (!Multiplayer::loggedIn)
		return {};
	for (int i = 0; i < Multiplayer::getEntites().size(); i++)
	{
		Entity en = Multiplayer::getEntites()[i];
		if (en.id == id)
			return en;
	}
	return {};
}

Player* Gameplay::findPlayerById(unsigned int id)
{
	if (!Multiplayer::loggedIn)
		return NULL;
	for (int i = 0; i < Game::getGlobalObjects()->size(); i++)
	{
		Object* obj = (*Game::getGlobalObjects())[i];

		if (obj->type != Player_e)
			continue;

		Player* p = (Player*)obj;
		if (!p)
			continue;
		if (p->mpEntity.id == id)
			return p;
	}
	return NULL;
}


Player* Gameplay::createPlayer(Entity en)
{
	if (!Game::containsObject(en.id))
	{
		std::cout << "Created " << en.username << " " << en.id << std::endl;
		Player* p = new Player(en.position.x, en.position.y);
		p->mpEntity = en;
		p->create();
		return p;
	}
	return findPlayerById(en.id);
}

void Gameplay::update(Events::updateEvent update)
{
	SDL_SetWindowTitle(update.window, "Gaming");

	for (int i = 0; i < entitiesToRemove->size(); i++)
	{
		Player* p = (*entitiesToRemove)[i];

		if (!p->hasStarted)
			continue;

		p->username->die();
		p->die();
	}

	for (int i = 0; i < bulletsToRemove->size(); i++)
	{
		Bullet* b = (*bulletsToRemove)[i];

		b->die();
	}

	bulletsToRemove->clear();
	entitiesToRemove->clear();

	Player* local = getLocalPlayer();

	if (local)
	{

		if (Multiplayer::loggedIn)
			getHud()->setText("FPS: " + boost::lexical_cast<std::string>(Game::gameFPS) + "\nAmmo: " + boost::lexical_cast<std::string>(local->ammo) + "\nHealth: " + boost::lexical_cast<std::string>(local->health));

		ImGui::BulletText("Objects: %i", Game::getGlobalObjects()->size());
		ImGui::BulletText("XVel: %f YVel: %f", local->xVel, local->yVel);
		ImGui::BulletText("X: %f Y: %f", local->x, local->y);
	}
}

