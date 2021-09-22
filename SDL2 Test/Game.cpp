#include "includes.h"
#include "Game.h"
using namespace std;


void Game::update(updateEvent update)
{
	SDL_SetWindowTitle(update.window, to_string(SDL_GetTicks()).append(" Ticks have passed").c_str());


	SDL_SetRenderDrawColor(update.renderer, 100, 100, 100, 255);
	SDL_RenderClear(update.renderer);
	SDL_RenderPresent(update.renderer);
}


void Game::keyDown(SDL_KeyboardEvent event)
{
	switch (event.keysym.sym)
	{
		case SDLK_w:
			cout << "pressed w\n";
		break;
		case SDLK_s:
			cout << "pressed s\n";
			break;
		case SDLK_d:
			cout << "pressed d\n";
			break;
		case SDLK_a:
			cout << "pressed a\n";
			break;
	}


}

void Game::keyUp(SDL_KeyboardEvent event)
{

}