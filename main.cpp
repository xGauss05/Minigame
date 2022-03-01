#include "SDL/include/SDL.h"

#include <stdio.h>
#include <iostream>

#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

using namespace std;

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

class SDLManager {
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
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("Unable to initialize SDL. Reason: %s", SDL_GetError());
		}
	}

	void windowInitializer(char* windowName) {

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
	
	void updateBackground() {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderClear(renderer); // Used to update the screen while clearing unnecesary objects
	}

	void updateFigure(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
		SDL_SetRenderDrawColor(renderer, r, g, b, a); // Prepares the renderer to paint with a color
		SDL_RenderFillRect(renderer, &rect); // Paints rect with the color previously assigned
	}

};

int main(int argc, char** argv) {
	SDLManager sdlManager;

	sdlManager.startSDLInitializer();

	SDL_Rect rect = { 0,430,50,50 }; // x, y, width, height
	SDL_Rect bullet;

	bool bulletShot = false;
	bool keep_window_open = true;
	while (keep_window_open) {
		// Red background
		sdlManager.updateBackground();
		// Blue square
		sdlManager.updateFigure(rect, 0, 0, 255, 255);
		if (bulletShot) { // If spacebar is pressed
			bullet.x = rect.x + 75;
			bullet.y = rect.y + 20;
			bullet.w = 75;
			bullet.h = 10;
			while (bullet.x <= WINDOW_WIDTH) {
				sdlManager.updateFigure(rect, 0,0,255,255);
				sdlManager.updateFigure(bullet, 0, 255, 0, 255);
				SDL_RenderPresent(sdlManager.renderer);
				bullet.x += 35;
				SDL_Delay(50);
				sdlManager.updateBackground();
			}
			bulletShot = false;
		}
		SDL_RenderPresent(sdlManager.renderer); // Updates the screen with any rendering process previously called
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
					bulletShot = true;
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
				}
				break;
			case SDL_KEYUP:
				//cout << "Key up detected!" << endl;
				break;
			}
		}
	}
	return 0;
}