#include "includes.h"
#include <algorithm>
#include "Multiplayer.h"
#include <Windows.h>

#include "Game.h"
#include "Bullet.h"
#include "Player.h"

using namespace std;

vector<Object*>* objects;

vector<Player*>* entitiesToRemove;
vector<Bullet*>* bulletsToRemove;

map<int, bool> Game::controls = {
	{SDLK_w, false},
	{SDLK_s, false},
	{SDLK_a, false},
	{SDLK_d, false},
	{SDLK_r, false}
};

SDL_Renderer* Game::renderer;
SDL_Window* Game::window;

double Game::gameFPS = 0.0;

float Game::deltaTime = 0;


bool contains(unsigned int id)
{
	for (int i = 0; i < objects->size(); i++)
	{
		Object* obj = (*objects)[i];

		if (obj->type != Player_e)
			continue;

		Player* p = (Player*)obj;

		if (!p)
			continue;
		if (p->mpEntity.id == id)
			return true;
	}
	return false;
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


void Game::createGame()
{
	objects = new std::vector<Object*>();
	entitiesToRemove = new std::vector<Player*>();
	bulletsToRemove = new std::vector<Bullet*>();
	CreateThread(NULL, NULL, Multiplayer::connect, NULL, NULL, NULL);
}


void updatePlayers()
{
	for (int i = 0; i < Multiplayer::getEntites().size(); i++)
	{
		Entity en = Multiplayer::getEntites()[i];

		Player* p;
		Bullet* b;

		switch (en.EntityType)
		{
			case 0:
				if (en.id == Multiplayer::localId)
					continue;

				p = Game::createPlayer(en);
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
						continue;

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
	}

	for (int i = 0; i < objects->size(); i++)
	{
		Object* obj = (*objects)[i];

		Player* p;
		Bullet* b;

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
	}

	for (int i = 0; i < Bullet::getBullets()->size(); i++)
	{
		Bullet* b = (*Bullet::getBullets())[i];
		if (!b)
			continue;

		if (!containsEntity(b->mpEntity.id))
		{
			bulletsToRemove->push_back(b);
		}
	}
}

void Game::onLoggedIn()
{
	getLocalPlayer();
	updatePlayers();
}

void Game::onUpdateGameState()
{
	if (Multiplayer::loggedIn)
		updatePlayers();
}



TextDisplay* Game::getHud()
{
	static TextDisplay* hud = nullptr;
	if (!hud)
	{
		hud = new TextDisplay(0, 0, "hello", 100, 100);
		hud->create();
	}
	return hud;

}

void Game::update(Events::updateEvent update)
{
	SDL_SetWindowTitle(update.window, to_string(objects->size()).append(" objects created").c_str());

	SDL_RenderClear(update.renderer);


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

	for (int i = 0; i < objects->size(); i++)
	{
		try
		{
			Object* fuck = (*objects)[i];
			fuck->update(update);
		}
		catch (...)
		{

		}
	}

	if (Multiplayer::loggedIn)
		getHud()->setText(("FPS: " + std::to_string(gameFPS) + "\nHealth: " + to_string(getLocalPlayer()->health) + "\nAmmo: " + to_string(getLocalPlayer()->ammo)));
	else
		getHud()->setText(("FPS: " + std::to_string(gameFPS) + "\nNot logged in"));

	SDL_SetRenderDrawColor(update.renderer, 10, 10, 10, 255);

	SDL_RenderPresent(update.renderer);
}


Player* Game::getLocalPlayer()
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
	}
	return p;
}

Entity Game::findEntityById(unsigned int id)
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

Player* Game::findPlayerById(unsigned int id)
{
	if (!Multiplayer::loggedIn)
		return NULL;
	for (int i = 0; i < objects->size(); i++)
	{
		Object* obj = (*objects)[i];

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


Player* Game::createPlayer(Entity en)
{
	if (!contains(en.id))
	{
		std::cout << "Created " << en.username << " " << en.id << std::endl;
		Player* p = new Player(en.position.x, en.position.y);
		p->mpEntity = en;
		p->create();
		return p;
	}
	return findPlayerById(en.id);
}


void Game::keyDown(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = true;
	for (int i = 0; i < objects->size(); i++)
	{
		Object* fuck = (*objects)[i];
		fuck->keyDown(ev);
	}
}

void Game::keyUp(SDL_KeyboardEvent ev)
{
	if (controls.count(ev.keysym.sym) == 1)
		controls[ev.keysym.sym] = false;
}

std::vector<Object*>* Game::getGlobalObjects()
{
	return objects;
}

void Game::addGlobalObject(Object* obj)
{
	objects->push_back(obj);
}

void Game::removeGlobalObject(Object* obj)
{
	objects->erase(std::remove(objects->begin(), objects->end(), obj), objects->end());
}

bool Game::getKey(int code)
{
	if (controls.count(code) != 1)
		return false;
	return controls[code];
}
