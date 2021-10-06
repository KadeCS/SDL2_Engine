#pragma once
#include "includes.h"
#include "Object.h"
class Level
{
	public:
		static Level* LoadLevel(std::string path);

		Level(std::vector<Object*>* objects);

		void clean();

		std::vector<Object*>* savedObjects;
};

