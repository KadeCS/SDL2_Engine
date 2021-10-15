#include "includes.h"
#include "Game.h"
#include <SDL_ttf.h>
#include "Utils.h"

using namespace std;

#undef main

#pragma comment(lib,"x86\\SDL2.lib")
#pragma comment(lib,"x86\\SDL2_ttf.lib")
#pragma comment(lib,"x86\\SDL2_image.lib")
#pragma comment(lib,"x86\\imgui.lib")
#pragma comment(lib,"x86\\implot.lib")

#define FRAME_VALUES 60

Uint32 frametimes[FRAME_VALUES];

Uint32 frametimelast;

Uint32 framecount;

void fpsinit() {
	memset(frametimes, 0, sizeof(frametimes));
	framecount = 0;
	frametimelast = SDL_GetTicks();
}

void fpsthink() {
	Uint32 frametimesindex;
	Uint32 getticks;
	Uint32 count;
	Uint32 i;

	frametimesindex = framecount % FRAME_VALUES;

	getticks = SDL_GetTicks();

	frametimes[frametimesindex] = getticks - frametimelast;

	frametimelast = getticks;

	framecount++;

	if (framecount < FRAME_VALUES)
		count = framecount;
	else
		count = FRAME_VALUES;

	Game::gameFPS = 0;
	for (i = 0; i < count; i++)
		Game::gameFPS += frametimes[i];

	Game::gameFPS /= count;
	Game::gameFPS = std::floorf(1000.f / Game::gameFPS);
}

int main(int argc, char* argv[])
{

	cout << "gaming";

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	fpsinit();

	SDL_Window* window = SDL_CreateWindow("Da window", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 792, 600, SDL_WINDOW_SHOWN);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGuiSDL::Initialize(renderer, 792, 600);

	ImGui::StyleColorsDark();

	ImPlot::GetStyle().AntiAliasedLines = true;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Game* game = new Game();

	SDL_StartTextInput();

	cout << "gaming";

	bool run = true;

	int lastTime = 0;

	float timestamps[30], fps[30], deltaTimes[30];

	for (int i = 0; i < 30; i++)
	{
		timestamps[i] = 0;
		fps[i] = 0;
		deltaTimes[i] = 0;
	}

	int frames = 0;

	bool create = false;
	while (run)
	{
		const Uint32 startTime = SDL_GetTicks();
		Game::startTick = startTime;
		SDL_Event event;
		int wheel = 0;

		ImGui::NewFrame();

		ImGui::Begin("Debug Window");
		ImGui::SetWindowSize(ImVec2(500,540));

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
			case SDL_MOUSEWHEEL:
				wheel = event.wheel.y;
				break;
			}

		}

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
		io.DeltaTime = Utils::clamp(Game::deltaTime, 1, 100);
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		io.MouseWheel = static_cast<float>(wheel);


		ImGui::Text("Performance Graph");

		ImPlot::SetNextPlotLimits(0, 30, 0, 160);
		if (ImPlot::BeginPlot("FPS", "Frame in snippet", "Data")) {
			ImPlot::PlotLine("FPS", timestamps, fps, 30);
			ImPlot::PlotLine("Delta Time", timestamps, deltaTimes, 30);
			ImPlot::EndPlot();
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

		Game::deltaTime = SDL_GetTicks() - startTime;

		fpsthink();


		ImGui::End();

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());

		SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);

		SDL_RenderPresent(renderer);

		if (1000 / 120 > SDL_GetTicks() - startTime) {
			SDL_Delay(1000 / 120 - (SDL_GetTicks() - startTime));
		}

		frames++;

		if (frames > 30)
			frames = 0;

		timestamps[frames] = frames;
		fps[frames] = Game::gameFPS;
		deltaTimes[frames] = Game::deltaTime;


	}


	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	SDL_StopTextInput();

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
