#pragma once

#ifndef MANAGER_H
#define MANAGER_H

#include "manager.h"
#include "SDL/include/SDL.h"

#pragma comment(lib, "SDL/libx86/SDL2.lib")
#pragma comment(lib, "SDL/libx86/SDL2main.lib")

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

class SDLManager {
private:
	SDL_Window* window;
	SDL_Surface* window_surface;

public:
	SDLManager();
	~SDLManager();
	SDL_Renderer* renderer;
	void initializer();

	void initSDL();

	void windowInitializer(char* windowName);

	void surfaceInitializer();

	void createRenderer();

	void updateBackground(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void updateFigure(SDL_Rect rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};
#endif
