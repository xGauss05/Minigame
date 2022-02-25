#include "SDL/include/SDL.h"

#include <stdio.h>
#include <iostream>

#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

using namespace std;

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

class SDLInitializer {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* window_surface;
	SDL_Rect* rect[];

	void startSDLInitializer() {
		char windowName[64];
		cout << "Input window name: " << endl;
		cin >> windowName;
		initSDL();
		windowInitializer(windowName);
		surfaceInitializer();
		createRenderer();
	}

	void initSDL() {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			SDL_Log("Unable to initialize SDL. Reason: %s", SDL_GetError());
		}
	}

	void windowInitializer(char* windowName) {
		
		window = SDL_CreateWindow(windowName,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN);

		if (!window) {
			SDL_Log("Unable to create window. Reason: %s", SDL_GetError());
		}
	}

	void surfaceInitializer() {
		window_surface = SDL_GetWindowSurface(window);
		if (!window_surface) {
			SDL_Log("Unable to create window surface. Reason: %s", SDL_GetError());
		}
		SDL_UpdateWindowSurface(window);
	}

	void createRenderer() {
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == NULL) {
			SDL_Log("Unable to create rendering context. Reason: %s", SDL_GetError());
		}
	}
};

int main(int argc, char** argv) {
	SDLInitializer sdlInit;

	sdlInit.startSDLInitializer();
	
	SDL_Rect rect;
	rect.x = 50;
	rect.y = 50;
	rect.w = 50;
	rect.h = 50;

	SDL_SetRenderDrawColor(sdlInit.renderer, 255, 0, 0, 255);
	SDL_RenderClear(sdlInit.renderer);

	bool keep_window_open = true;
	while (keep_window_open) {
		// Actualiza el fondo a color rojo
		/*SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderClear(renderer);*/

		// Actualiza el color del cuadrado
		SDL_SetRenderDrawColor(sdlInit.renderer, 0, 0, 255, 255);
		SDL_RenderFillRect(sdlInit.renderer, &rect);
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0) {
			switch (e.type) {
			case SDL_QUIT:
				keep_window_open = false;
				break;
			case SDL_KEYDOWN:
				//cout << "Key down detected!" << endl;
				switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					// Repaint background to red
					SDL_SetRenderDrawColor(sdlInit.renderer, 255, 0, 0, 255);
					SDL_RenderClear(sdlInit.renderer);
					break;
				case SDLK_ESCAPE:
					keep_window_open = false;
					break;
				case SDLK_LEFT:
					rect.x -= 10;
					break;
				case SDLK_RIGHT:
					rect.x += 10;
					break;
				case SDLK_UP:
					rect.y -= 10;
					break;
				case SDLK_DOWN:
					rect.y += 10;
					break;
				default:
					break;
				}
				SDL_RenderPresent(sdlInit.renderer);
				break;
			case SDL_KEYUP:
				//cout << "Key up detected!" << endl;
				break;
			}
			SDL_UpdateWindowSurface(sdlInit.window);
		}
		SDL_RenderPresent(sdlInit.renderer);
	}

	return 0;
}