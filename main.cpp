#include "manager.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#define WINDOW_WIDTH	640
#define WINDOW_HEIGHT	480

// Animation  sprites
// https://caveofprogramming.com/guest-posts/animating-using-sprite-sheets.html

int main(int argc, char* args[]) {
	SDLManager sdlManager;

	sdlManager.initializer();

	SDL_Rect blueSquare = { 0,430,50,50 }; // x, y, width, height
	SDL_Rect bullet;

	bool bulletShot = false;
	bool keep_window_open = true;
	while (keep_window_open) {
		// Red background
		sdlManager.updateBackground(255, 0, 0, 255);
		// Blue square
		sdlManager.updateFigure(blueSquare, 0, 0, 255, 255);
		if (bulletShot) { // If spacebar is pressed
			bullet = { blueSquare.x + 75 , blueSquare.y + 20, 75, 10 };
			while (bullet.x <= WINDOW_WIDTH) { // Until the bullet reaches the width of the window
				sdlManager.updateFigure(blueSquare, 0, 0, 255, 255);
				sdlManager.updateFigure(bullet, 0, 255, 0, 255);
				SDL_RenderPresent(sdlManager.renderer);
				bullet.x += 25;
				sdlManager.updateBackground(255, 0, 0, 255);
				SDL_Delay(7);
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
					blueSquare.x -= 10;
					break;
				case SDLK_RIGHT:
					blueSquare.x += 10;
					break;
				case SDLK_UP:
					blueSquare.y -= 10;
					break;
				case SDLK_DOWN:
					blueSquare.y += 10;
					break;
				}
				break;
				//case SDL_KEYUP:
				//	cout << "Key up detected!" << endl;
				//	break;
			}
		}
	}
	return 0;
}