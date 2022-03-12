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

#define BUTTON_H		82
#define BUTTON_W		104
#define BUTTON_Y		WINDOW_HEIGHT - BUTTON_H
#define BUTTON_X		200
#define BUTTON_MARGIN   20

#define SPRITE_H		56
#define SPRITE_W		20
#define SPRITE_Y_SPAWN  -SPRITE_H
#define SPRITE_SPEED	4

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
	SDL_Rect error_marginW, error_marginA, error_marginS, error_marginD;
	Entity buttonW, buttonA, buttonS, buttonD, beats[MAX_BEATS];
	int idx_beat, idx_lowest_beat, column;

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};
