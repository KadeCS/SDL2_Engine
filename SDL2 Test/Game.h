#pragma once
class Game
{
	public:
		struct updateEvent {
			SDL_Renderer* renderer;
			SDL_Window* window;
		};
		void update(updateEvent updateEvent);
		void keyDown(SDL_KeyboardEvent event);
		void keyUp(SDL_KeyboardEvent event);
};

