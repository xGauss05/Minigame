#include "manager.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

Manager::Manager() {}
Manager::~Manager() {}

void initRect(SDL_Rect* button, int x, int y, int w, int h) {
	button->x = x;
	button->y = y;
	button->w = w;
	button->h = h;
}

bool Manager::init() {
	srand(time(NULL));

	// Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("(SDL_INIT_EVERYTHING) Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create our window: title, x, y, w, h, flags
	window = SDL_CreateWindow("Guitar Hero: Z, X, N, M or arrows", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_Log("(SDL_CreateWindow) Unable to create window: %s", SDL_GetError());
		return false;
	}

	// Create a 2D rendering context for a window: window, device index, flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		SDL_Log("(SDL_CreateRenderer) Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	// Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i) keys[i] = KEY_IDLE;

	// Load all textures
	if (!loadTextures()) return false;

	// Load all sounds
	if (!loadSounds()) return false;

	// Init variables
	Mix_PlayMusic(bgm, -1);
	buttonA.init(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonS.init(BUTTON_X + BUTTON_MARGIN + BUTTON_W, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonW.init(BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonD.init(BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3, BUTTON_Y, BUTTON_W, BUTTON_H, 0);

	initRect(&error_marginA, BUTTON_X, BUTTON_Y - BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginS, BUTTON_X + BUTTON_MARGIN + BUTTON_W, BUTTON_Y - BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginW, BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2, BUTTON_Y - BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginD, BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3, BUTTON_Y - BUTTON_H, BUTTON_W, BUTTON_H);

	idx_beat = 0;
	idx_lowest_beat = 0;

	delayCounter = 0;
	return true;
}

bool Manager::loadSounds() {
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		SDL_Log("(SDL_INIT_AUDIO) Failed to load soundS: %s\n", SDL_GetError());
		return false;
	}
	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	bgm = Mix_LoadMUS("sounds/bgm/bergentruckung.ogg");
	if (!bgm) {
		SDL_Log("(Mix_LoadMus) Couldn't load 'bgm': %s\n", Mix_GetError());
	}


	beat_sfx = Mix_LoadWAV("sounds/sfx/128.wav");
	if (!beat_sfx) {
		SDL_Log("(Mix_LoadWAV) Couldn't load 'beat_sfx': %s\n", Mix_GetError());
	}

	score_sfx = Mix_LoadWAV("sounds/sfx/score.wav");
	if (!score_sfx) {
		SDL_Log("(Mix_LoadWAV) Couldn't load 'score_sfx': %s\n", Mix_GetError());
	}

	fail_sfx = Mix_LoadWAV("sounds/sfx/oof.wav");
	if (!fail_sfx) {
		SDL_Log("(Mix_LoadWAV) Couldn't load 'fail_sfx': %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(100);

	return true;
}

bool Manager::loadTextures() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("IMG_Init: Failed to init required jpg and png support!\n");
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
	}

	left_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/left-arrow-framed.png"));
	if (left_arrow_btn_img == NULL) {
		SDL_Log("Couldn't load texture for left_arrow_btn_img: %s", SDL_GetError());
		return false;
	}
	left_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/left-arrow.png"));
	if (left_arrow_beat_img == NULL) {
		SDL_Log("Couldn't load texture for left_arrow_beat_img: %s", SDL_GetError());
		return false;
	}

	down_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/down-arrow-framed.png"));
	if (down_arrow_btn_img == NULL) {
		SDL_Log("Couldn't load texture for down_arrow_btn_img: %s", SDL_GetError());
		return false;
	}
	down_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/down-arrow.png"));
	if (down_arrow_beat_img == NULL) {
		SDL_Log("Couldn't load texture for down_arrow_beat_img: %s", SDL_GetError());
		return false;
	}

	up_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/up-arrow-framed.png"));
	if (up_arrow_btn_img == NULL) {
		SDL_Log("Couldn't load texture for up_arrow_btn_img: %s", SDL_GetError());
		return false;
	}
	up_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/up-arrow.png"));
	if (up_arrow_beat_img == NULL) {
		SDL_Log("Couldn't load texture for up_arrow_beat_img: %s", SDL_GetError());
		return false;
	}

	right_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/right-arrow-framed.png"));
	if (right_arrow_btn_img == NULL) {
		SDL_Log("Couldn't load texture for right_arrow_btn_img: %s", SDL_GetError());
		return false;
	}
	right_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/right-arrow.png"));
	if (right_arrow_beat_img == NULL) {
		SDL_Log("Couldn't load texture for right_arrow_beat_img: %s", SDL_GetError());
		return false;
	}

	background_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/bg.png"));
	if (background_img == NULL) {
		SDL_Log("Couldn't load texture for background_img: %s", SDL_GetError());
		return false;
	}

	player_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/spaceship.png"));
	if (player_img == NULL) {
		SDL_Log("Couldn't load texture for player_img: %s", SDL_GetError());
		return false;
	}

	beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/enemyshot.png"));
	if (beat_img == NULL) {
		SDL_Log("Couldn't load texture for beat_img: %s", SDL_GetError());
		return false;
	}

	// SDL_Surface* surface = IMG_Load("img/shot.png");
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
	delayCounter++;
	if ((rand() % (100 - 0 + 1) + 0) <= 10) {
		int randCol = (rand() % (4 - 1 + 1)) + 1;

		if (!beats[idx_beat].isAlive() && delayCounter >= DELAY) {
			SDL_Rect button;
			switch (randCol) {
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

			beats[idx_beat].init(button.x, SPRITE_Y_SPAWN, SPRITE_W, SPRITE_H, SPRITE_SPEED);

			idx_beat++;
			idx_beat %= MAX_BEATS;
			delayCounter = 0;
		}
	}

	// Logic
	// Beats update & input
	for (int i = 0; i < MAX_BEATS; ++i) {
		if (beats[i].isAlive()) {
			beats[i].move(0, 1);
			SDL_Rect button, beat;
			beats[i].getRect(&beat.x, &beat.y, &beat.w, &beat.h);
			if (keys[SDL_SCANCODE_Z] == KEY_DOWN || keys[SDL_SCANCODE_LEFT] == KEY_DOWN) {
				buttonA.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginA)) {
					beats[i].shutDown();
					Mix_PlayChannel(-1, fail_sfx, 0);
					break;
				}
			}

			if (keys[SDL_SCANCODE_X] == KEY_DOWN || keys[SDL_SCANCODE_DOWN] == KEY_DOWN) {
				buttonS.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginS)) {
					beats[i].shutDown();
					Mix_PlayChannel(-1, fail_sfx, 0);
					break;
				}
			}

			if (keys[SDL_SCANCODE_N] == KEY_DOWN || keys[SDL_SCANCODE_UP] == KEY_DOWN) {
				buttonW.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginW)) {
					beats[i].shutDown();
					Mix_PlayChannel(-1, fail_sfx, 0);
					break;
				}
			}

			if (keys[SDL_SCANCODE_M] == KEY_DOWN || keys[SDL_SCANCODE_RIGHT] == KEY_DOWN) {
				buttonD.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginD)) {
					beats[i].shutDown();
					Mix_PlayChannel(-1, fail_sfx, 0);
					break;
				}
			}

			if (SDL_HasIntersection(&beat, &button)) {
				beats[i].shutDown();
				Mix_PlayChannel(-1, score_sfx, 0);
				break;
			}

			if (beats[i].getY() >= WINDOW_HEIGHT) {
				beats[i].shutDown();
				Mix_PlayChannel(-1, fail_sfx, 0);
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
	SDL_RenderCopy(renderer, up_arrow_btn_img, NULL, &rc);

	buttonA.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, left_arrow_btn_img, NULL, &rc);

	buttonS.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, down_arrow_btn_img, NULL, &rc);

	buttonD.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, right_arrow_btn_img, NULL, &rc);

	// Draw beats
	for (int i = 0; i < MAX_BEATS; ++i) {
		if (beats[i].isAlive()) {
			beats[i].getRect(&rc.x, &rc.y, &rc.w, &rc.h);
			if (beats[i].getX() == BUTTON_X) {
				SDL_RenderCopy(renderer, left_arrow_beat_img, NULL, &rc);
			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN + BUTTON_W) {
				SDL_RenderCopy(renderer, down_arrow_beat_img, NULL, &rc);

			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2) {
				SDL_RenderCopy(renderer, up_arrow_beat_img, NULL, &rc);

			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3) {
				SDL_RenderCopy(renderer, right_arrow_beat_img, NULL, &rc);

			}
		}
	}

	// Update screen
	SDL_RenderPresent(renderer);
	SDL_Delay(10);	// 1000/10 = 100 fps max
}
