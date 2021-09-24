#include "includes.h"
#include "Game.h"
using namespace std;

#undef main

#pragma comment(lib,"C:/Users/millk290/source/repos/SDL2 Test/SDL2 Test/lib/x86/SDL2.lib")


int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);


	SDL_Window* window = SDL_CreateWindow("Da window", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);


	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	Game* game = new Game();

	SDL_StartTextInput();

	bool run = true;
	while (run)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type) {
			case SDL_QUIT:
				run = false;
				break;
			case SDL_KEYDOWN:
				game->keyDown(event.key);
				break;
			case SDL_KEYUP:
				game->keyUp(event.key);
				break;
			}

		}

		Events::updateEvent updateEvent;

		updateEvent.renderer = renderer;
		updateEvent.window = window;

		game->update(updateEvent);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}