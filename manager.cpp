#include "manager.h"
#include "SDL/include/SDL.h"

#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480
SDLManager::SDLManager() {}
SDLManager::~SDLManager() {}

void SDLManager::initializer() {
	char windowName[64];
	/*cout << "Input window name: " << endl;
	cin >> windowName;*/
	initSDL();
	windowInitializer(windowName);
	surfaceInitializer();
	createRenderer();
}

void SDLManager::initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL. Reason: %s", SDL_GetError());
	}
}

void SDLManager::windowInitializer(char* windowName) {
	window = SDL_CreateWindow("Tests",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN);

	if (!window) {
		SDL_Log("Unable to create window. Reason: %s", SDL_GetError());
	}
}

void SDLManager::surfaceInitializer() {
	window_surface = SDL_GetWindowSurface(window);
	if (!window_surface) {
		SDL_Log("Unable to create window surface. Reason: %s", SDL_GetError());
	}
	//SDL_UpdateWindowSurface(window);
}

void SDLManager::createRenderer() {
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		SDL_Log("Unable to create rendering context. Reason: %s", SDL_GetError());
	}
}

void SDLManager::updateBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer); // Used to update the screen while clearing unnecesary objects
}

void SDLManager::updateFigure(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_SetRenderDrawColor(renderer, r, g, b, a); // Prepares the renderer to paint with a color
	SDL_RenderFillRect(renderer, &rect); // Paints rect with the color previously assigned
}
