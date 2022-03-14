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

#define BUTTON_H		96
#define BUTTON_W		96
#define BUTTON_Y		WINDOW_HEIGHT - BUTTON_H
#define BUTTON_X		200
#define BUTTON_MARGIN   20

#define SPRITE_H		48
#define SPRITE_W		48
#define SPRITE_Y_SPAWN  -SPRITE_H
#define SPRITE_SPEED	7

#define SPAWN_DELAY		25	

#define SCORE_X			100
#define SCORE_Y			100
#define SCORE_W			50
#define SCORE_H			WINDOW_HEIGHT - 120

#define SCORE			5

class Manager {
public:
	Manager();
	~Manager();

	bool init();
	bool initSDL();
	bool loadSounds();
	bool loadTextures();
	bool initVariables();
	void release();
	bool input();
	bool update();
	void draw();

private:
	// SDL
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Textures
	SDL_Texture* background_img, * score_img, * remainingScore_img, * end_img, * start_img,
		* left_arrow_btn_img, * down_arrow_btn_img, * up_arrow_btn_img, * right_arrow_btn_img,
		* left_arrow_beat_img, * down_arrow_beat_img, * up_arrow_beat_img, * right_arrow_beat_img;

	// Sound
	Mix_Music* bgm;
	Mix_Chunk* score_sfx, * fail_sfx;

	// Entities
	SDL_Rect error_marginW, error_marginA, error_marginS, error_marginD;
	Entity buttonW, buttonA, buttonS, buttonD, beats[MAX_BEATS], score, remainingScore;

	// Counters
	int idx_beat, delayCounter;

	// Boolean
	bool gameStart, gameEnd, debugMode;

	// Input
	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP };
	KEY_STATE keys[MAX_KEYS];
};
