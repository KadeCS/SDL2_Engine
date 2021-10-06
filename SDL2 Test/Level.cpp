#include "Level.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Wall.h"
Level* Level::LoadLevel(std::string path)
{
	std::string complete = "";
	std::fstream newfile;
	newfile.open(path, std::ios::in);
	if (newfile.is_open()) {
		std::string tp;
		while (getline(newfile, tp)) { 
			complete.append(tp);
		}
		newfile.close();
	}

	auto j = nlohmann::json::parse(complete);

	std::vector<Object*>* objects = new std::vector<Object*>();

	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		Wall* w = new Wall((*it)["wall"]["x"], (*it)["wall"]["y"]);
		objects->push_back(w);
	}

	Level* l = new Level(objects);

	return l;
}

void Level::clean()
{
	for (int i = 0; i < savedObjects->size(); i++)
	{
		Object* obj = (*savedObjects)[i];

		obj->die();
	}
}

Level::Level(std::vector<Object*>* objects)
{
	savedObjects = objects;
}
