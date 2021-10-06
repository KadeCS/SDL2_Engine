#include "includes.h"
#include "Game.h"
#include <SDL_ttf.h>
using namespace std;

#undef main

#pragma comment(lib,"x86\\SDL2.lib")
#pragma comment(lib,"x86\\SDL2_ttf.lib")
#pragma comment(lib,"x86\\SDL2_image.lib")

int main(int argc, char* argv[])
{

	cout << "gaming";

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	SDL_Window* window = SDL_CreateWindow("Da window", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Game* game = new Game();

	SDL_StartTextInput();

	cout << "gaming";

	bool run = true;

	bool create = false;

	long numFrames = 0;
	long totalTime = 0;
	while (run)
	{
		const Uint32 startTime = SDL_GetTicks();
		Uint64 start = SDL_GetPerformanceCounter();
		SDL_Event event;
		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type) {
			case SDL_QUIT:
				Multiplayer::inQuotesGracefullyDisconnect();
				run = false;
				break;
			case SDL_KEYDOWN:
				game->keyDown(event.key);
				break;
			case SDL_KEYUP:
				game->keyUp(event.key);
				break;
			case SDL_TEXTINPUT:
				game->textInput(event.text);
				break;
			}

		}

		Events::updateEvent updateEvent;

		updateEvent.renderer = renderer;
		updateEvent.window = window;

		if (!create)
		{
			Game::renderer = renderer;
			Game::window = window;
			game->createGame();
			create = true;
		}
		game->update(updateEvent);
		Uint64 end = SDL_GetPerformanceCounter();

		const Uint32 endTime = SDL_GetTicks();

		totalTime += endTime - startTime;

		++numFrames;
		Game::gameFPS = numFrames / (totalTime / 1000.0);

		Game::deltaTime = endTime - startTime;

		
		if (SDL_GetTicks() % 1000 == 0)
		{
			numFrames = 0;
			totalTime = 0;
		}

	}


	SDL_StopTextInput();

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}