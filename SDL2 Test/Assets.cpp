#include "Assets.h"

#include <windows.h>

Asset* Assets::getAsset(std::string path, SDL_Renderer* renderer)
{

	TCHAR NPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, NPath);

	std::string p = NPath;
	p.append("\\assets\\");
	p.append(path.c_str());

	std::cout << "Getting asset " << p << std::endl;

	SDL_Texture* texture = IMG_LoadTexture(renderer, p.c_str());

	int w = 0;
	int h = 0;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	Asset* a = new Asset(texture, w, h);

	std::cout << "w&h" << a->w << " " << a->h << std::endl;
	return a;
}
