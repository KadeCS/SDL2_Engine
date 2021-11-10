#include "LevelEditor.h"
#include "Utils.h"
#include <windows.h>
#include "json.hpp"
#include <fstream>
#include "Level.h"

bool LevelEditor::pressedM;

bool removeWall = false;

SDL_FRect highlight;

void LevelEditor::update(Events::updateEvent event)
{
	int m_x, m_y;

	Uint32 buttons;

	SDL_PumpEvents();

	buttons = SDL_GetMouseState(&m_x, &m_y);

	m_x = Utils::Align(m_x, 24);
	m_y = Utils::Align(m_y, 24);

	highlight.x = m_x;
	highlight.y = m_y;

	highlight.w = 24;
	highlight.h = 24;

	SDL_SetRenderDrawColor(event.renderer, 255, 255, 255, 128);

	SDL_RenderFillRectF(event.renderer, &highlight);

	if ((buttons & SDL_BUTTON_LMASK) != 0)
	{

		bool place = true;

		if (removeWall)
		{
			place = false;
		}

		for (int i = 0; i < vectorOfWall->size(); i++)
		{
			Wall* w = (*vectorOfWall)[i];
			int xx = w->x;
			int yy = w->y;

			if (xx == m_x && yy == m_y)
			{
				if (removeWall)
				{
					std::cout << "removed wall " << w->x << "," << w->y << std::endl;
					w->die();
					vectorOfWall->erase(std::remove(vectorOfWall->begin(), vectorOfWall->end(), w), vectorOfWall->end());
					delete w;
				}
				else
					place = false;
			}

		}

		if (place)
		{

			Wall* wall = new Wall(m_x, m_y);
			wall->create();

			vectorOfWall->push_back(wall);

			std::cout << "making wall " << wall->x << "," << wall->y << std::endl;
		}
	}


	std::string hudText = "Level Editor v1";

	hudText.append((removeWall ? "\ndelete walls (holding shift)" : "\nadding walls (hold shift to remove)"));

	getHud()->setText(hudText);
}


void LevelEditor::saveLevel()
{
	nlohmann::json arra = nlohmann::json::array();

	for (int i = 0; i < vectorOfWall->size(); i++)
	{
		Wall* w = (*vectorOfWall)[i];

		nlohmann::json j;
		nlohmann::json wall;

		wall["x"] = w->x;
		wall["y"] = w->y;

		j["wall"] = wall;

		arra.push_back(j);
	}

	std::ofstream level;
	level.open("lvl.level");
	level << arra.dump();
	level.close();
}

Level* currentLoaded = nullptr;

void LevelEditor::loadLevel()
{
	if (currentLoaded != nullptr)
	{
		currentLoaded->clean();
		delete currentLoaded;
	}

	currentLoaded = Level::LoadLevel("lvl.level");

	for (int i = 0; i < vectorOfWall->size(); i++)
	{
		Wall* w = (*vectorOfWall)[i];
		w->die();
		vectorOfWall->erase(std::remove(vectorOfWall->begin(), vectorOfWall->end(), w), vectorOfWall->end());
		delete w;
	}

	for (int i = 0; i < currentLoaded->savedObjects->size(); i++)
	{
		Wall* w = (Wall*)(*currentLoaded->savedObjects)[i];
		w->create();

		vectorOfWall->push_back(w);
	}
}

void LevelEditor::keyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_LSHIFT:
			removeWall = true;
			break;
		case SDLK_s:
			saveLevel();
			MessageBoxA(NULL, "Saved to lvl.level", "Game Engine", NULL);
			break;
		case SDLK_l:
			loadLevel();
			break;
	}
}

void LevelEditor::keyUp(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_LSHIFT:
			removeWall = false;
			break;
	}
}


TextDisplay* LevelEditor::getHud()
{
	static TextDisplay* hud = nullptr;
	if (!hud)
	{
		hud = new TextDisplay(0, 0, "hello", 100, 100);
		hud->create();
	}
	return hud;

}