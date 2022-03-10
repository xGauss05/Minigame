#include "manager.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

Manager::Manager() {}
Manager::~Manager() {}

bool Manager::init() {
	srand(time(NULL));

	// Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create our window: title, x, y, w, h, flags
	window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	// Create a 2D rendering context for a window: window, device index, flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	// Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i) keys[i] = KEY_IDLE;

	if (!loadTextures()) return false;

	if (!loadSounds()) return false;

	// Init variables

	buttonA.init(20, WINDOW_HEIGHT - 82, 104, 82, 0);
	buttonS.init(40 + 104, WINDOW_HEIGHT - 82, 104, 82, 0);
	buttonW.init(60 + 208, WINDOW_HEIGHT - 82, 104, 82, 0);
	buttonD.init(80 + 312, WINDOW_HEIGHT - 82, 104, 82, 0);

	idx_beat = 0;

	return true;
}

bool Manager::loadSounds() {
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		SDL_Log("Failed to load sound: %s\n", SDL_GetError());
		return false;
	}
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	bgm = Mix_LoadMUS("sounds/bgm/106.ogg");
	if (!bgm) {
		SDL_Log("Mix_LoadWAV: %s\n", Mix_GetError());
	}
	Mix_AllocateChannels(100);
	//Mix_PlayMusic(music,-1);
	beat_sfx = Mix_LoadWAV("sounds/sfx/128.wav");
	oof_sfx = Mix_LoadWAV("sounds/sfx/oof.wav");
	//sound2 = Mix_LoadWAV("sounds/sfx/182.wav");
	return true;
}

bool Manager::loadTextures() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("IMG_Init: Failed to init required jpg and png support!\n");
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
	}

	background_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("smile.png"));
	if (background_img == NULL) {
		SDL_Log("Couldn't load texture for background_img: %s", SDL_GetError());
		return false;
	}

	player_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("spaceship.png"));
	if (player_img == NULL) {
		SDL_Log("Couldn't load texture for player_img: %s", SDL_GetError());
		return false;
	}

	beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("shot.png"));
	if (beat_img == NULL) {
		SDL_Log("Couldn't load texture for shot_img: %s", SDL_GetError());
		return false;
	}

	SDL_Surface* surface = IMG_Load("shot.png");
	return true;
}

void Manager::release() {
	SDL_DestroyTexture(background_img);
	SDL_DestroyTexture(player_img);
	SDL_DestroyTexture(beat_img);
	IMG_Quit();
	Mix_FreeMusic(bgm);
	Mix_FreeChunk(beat_sfx);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

bool Manager::input() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keyboard[i]) {
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		}
		else {
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
		}
	}

	return true;
}

bool Manager::update() {
	// Read Input
	if (!input()) return true;

	// Process Input
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN) return true;
	if ((rand() % (100 - 0 + 1) + 0) <= 5) {
		if (!beats[idx_beat].isAlive()) {
			SDL_Rect button;
			switch ((rand() % (4 - 1 + 1)) + 1) {

			case 1:
				buttonA.getRect(&button.x, &button.y, &button.w, &button.h);
				break;
			case 2:
				buttonS.getRect(&button.x, &button.y, &button.w, &button.h);
				break;
			case 3:
				buttonW.getRect(&button.x, &button.y, &button.w, &button.h);
				break;
			case 4:
				buttonD.getRect(&button.x, &button.y, &button.w, &button.h);
				break;
			}

			beats[idx_beat].init(button.x, -82, 56, 20, 5);
			idx_beat++;
			idx_beat %= MAX_BEATS;
		}
	}
	// Logic
	// Beats update & input
	for (int i = 0; i < MAX_BEATS; ++i) {
		if (beats[i].isAlive()) {
			beats[i].move(0, 1);
			if (beats[i].getY() >= WINDOW_HEIGHT) beats[i].shutDown();

			SDL_Rect button, beat;
			beats[i].getRect(&beat.x, &beat.y, &beat.w, &beat.h);
			if (keys[SDL_SCANCODE_Z] == KEY_DOWN) {
				buttonA.getRect(&button.x, &button.y, &button.w, &button.h);
			}

			if (keys[SDL_SCANCODE_X] == KEY_DOWN) {
				buttonS.getRect(&button.x, &button.y, &button.w, &button.h);
			}

			if (keys[SDL_SCANCODE_N] == KEY_DOWN) {
				buttonW.getRect(&button.x, &button.y, &button.w, &button.h);
			}

			if (keys[SDL_SCANCODE_M] == KEY_DOWN) {
				buttonD.getRect(&button.x, &button.y, &button.w, &button.h);
			}

			if (SDL_HasIntersection(&beat, &button)) { 
				beats[i].shutDown();
				Mix_PlayChannel(-1, oof_sfx, 0);
			}
		}
	}

	return false;
}

void Manager::draw() {
	// Clears the renderer
	SDL_RenderClear(renderer);

	// Draw background
	SDL_RenderCopy(renderer, background_img, NULL, NULL);

	// Draw buttons
	SDL_Rect rc;
	buttonW.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);

	buttonA.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);

	buttonS.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);

	buttonD.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);

	// Draw beats
	for (int i = 0; i < MAX_BEATS; ++i) {
		if (beats[i].isAlive()) {
			beats[i].getRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(renderer, beat_img, NULL, &rc);
		}
	}

	// Update screen
	SDL_RenderPresent(renderer);
	SDL_Delay(10);	// 1000/10 = 100 fps max
}
