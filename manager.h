#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

#include "entity.h"

#define WINDOW_WIDTH	1280
#define WINDOW_HEIGHT	720
#define MAX_KEYS		256
#define MAX_BEATS		32

#define COL_LEFT_X
#define COL_DOWN_X
#define COL_UP_X
#define COL_RIGHT_X

class Manager {
public:
	Manager();
	~Manager();

	bool init();
	bool loadSounds();
	bool loadTextures();
	void release();

	bool input();
	bool update();
	void draw();

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* background_img, * player_img, * beat_img;
	Mix_Music* bgm;
	Mix_Chunk* beat_sfx, *score_sfx, *fail_sfx;
	
	Entity buttonW, buttonA, buttonS, buttonD, beats[MAX_BEATS];
	int idx_beat, idx_lowest_beat;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};
