#include "manager.h"
#include <math.h>
#include <time.h>
#include <stdlib.h>

Manager::Manager() {}
Manager::~Manager() {}

/**
 * This function computes a random number between max and min, both included
 * @param max the maximum parameter
 * @param min the minimum parameter
 * @return Random number between max and min.
 */
int random(int max, int min) {
	return (rand() % (max - min + 1)) + min;
}

/**
 * This function initializes a button's rect
 * @param button the Rect to be initialized
 * @param x coordinate x
 * @param y coordinate y
 * @param w width of the rect
 * @param h height of the rect
 */
void initRect(SDL_Rect* button, int x, int y, int w, int h) {
	button->x = x;
	button->y = y;
	button->w = w;
	button->h = h;
}

/**
 * This function updates the scorebar
 * @param score the entity to be updated (score)
 * @param hasScored parameter to check if it has scored or not
 * @param sound the sound to be played
 */
void updateScore(Entity* score, bool hasScored, Mix_Chunk* sound) {
	if (hasScored) {
		// Moves the bar up
		score->move(0, -SCORE);
		// Updates the height
		score->updateHeight(SCORE);
	}
	else {

		if (score->getY() <= SCORE_H) {
			// moves the bar down+
			score->move(0, SCORE);
			// Updates the height 
			score->updateHeight(-SCORE);
		}
	}
	Mix_PlayChannel(-1, sound, 0);
}

/**
 * This function initializes all the resources necessary for the Game
 */
bool Manager::init() {
	srand(time(NULL));

	//Initialize SDL and subsystems
	if (!initSDL()) return false;

	// Initialize image/sprite system and load all textures
	if (!loadTextures()) return false;

	// Initialize sound system and load all sounds
	if (!loadSounds()) return false;

	// Initialize all of the variables needed for the game to work properly
	if (!initVariables()) return false;

	// Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i) keys[i] = KEY_IDLE;

	return true;
}

/**
 * This function initializes SDL related resources
 */
bool Manager::initSDL() {
	// Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("(SDL_INIT_EVERYTHING) Unable to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// Create our window: title, x, y, w, h, flags
	window = SDL_CreateWindow("Guitar Hero: A, S, W, D or arrows (F4 for debug mode)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_Log("(SDL_CreateWindow) Unable to create window: %s\n", SDL_GetError());
		return false;
	}

	// Create a 2D rendering context for a window: window, device index, flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		SDL_Log("(SDL_CreateRenderer) Unable to create rendering context: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

/**
 * This function initializes all sound related resources
 */
bool Manager::loadSounds() {
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		SDL_Log("(SDL_INIT_AUDIO) Failed to load sounds: %s\n", SDL_GetError());
		return false;
	}

	int flags = MIX_INIT_OGG;
	int initted = Mix_Init(flags);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	bgm = Mix_LoadMUS("sounds/bgm/bergentruckung.ogg");
	if (!bgm) {
		SDL_Log("(Mix_LoadMus) Couldn't load 'bgm': %s\n", Mix_GetError());
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
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	return true;
}

/**
 * This function initializes all texture related resources
 */
bool Manager::loadTextures() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("(IMG_Init) Failed to init required jpg and png support: %s\n", IMG_GetError());
	}

	left_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/left-arrow-framed.png"));
	if (left_arrow_btn_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for left_arrow_btn_img: %s\n", SDL_GetError());
		return false;
	}
	left_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/left-arrow.png"));
	if (left_arrow_beat_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for left_arrow_beat_img: %s\n", SDL_GetError());
		return false;
	}

	down_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/down-arrow-framed.png"));
	if (down_arrow_btn_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for down_arrow_btn_img: %s\n", SDL_GetError());
		return false;
	}
	down_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/down-arrow.png"));
	if (down_arrow_beat_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for down_arrow_beat_img: %s\n", SDL_GetError());
		return false;
	}

	up_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/up-arrow-framed.png"));
	if (up_arrow_btn_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for up_arrow_btn_img: %s\n", SDL_GetError());
		return false;
	}
	up_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/up-arrow.png"));
	if (up_arrow_beat_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for up_arrow_beat_img: %s\n", SDL_GetError());
		return false;
	}

	right_arrow_btn_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/right-arrow-framed.png"));
	if (right_arrow_btn_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for right_arrow_btn_img: %s\n", SDL_GetError());
		return false;
	}
	right_arrow_beat_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/right-arrow.png"));
	if (right_arrow_beat_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for right_arrow_beat_img: %s\n", SDL_GetError());
		return false;
	}

	background_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/bg.png"));
	if (background_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for background_img: %s\n", SDL_GetError());
		return false;
	}

	start_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/start.png"));
	if (start_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for start_img: %s\n", SDL_GetError());
		return false;
	}
	end_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/end.png"));
	if (end_img == NULL) {
		SDL_Log("(IMG_Load) Couldn't load texture for end_img: %s\n", SDL_GetError());
		return false;
	}

	remainingScore_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/remainingScore.png"));
	if (remainingScore_img == NULL) {
		SDL_Log("Couldn't load texture for background_img: %s\n", SDL_GetError());
		return false;
	}

	score_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("img/score.png"));
	if (score_img == NULL) {
		SDL_Log("Couldn't load texture for background_img: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

/**
 * This function initializes all the necessary variables
 */
bool Manager::initVariables() {
	// BGM Music volume

	Mix_PlayMusic(bgm, -1);

	buttonA.init(BUTTON_X, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonS.init(BUTTON_X + BUTTON_MARGIN + BUTTON_W, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonW.init(BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2, BUTTON_Y, BUTTON_W, BUTTON_H, 0);
	buttonD.init(BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3, BUTTON_Y, BUTTON_W, BUTTON_H, 0);

	initRect(&error_marginA, BUTTON_X, BUTTON_Y - 1.25 * BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginS, BUTTON_X + BUTTON_MARGIN + BUTTON_W, BUTTON_Y - 1.25 * BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginW, BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2, BUTTON_Y - 1.25 * BUTTON_H, BUTTON_W, BUTTON_H);
	initRect(&error_marginD, BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3, BUTTON_Y - 1.25 * BUTTON_H, BUTTON_W, BUTTON_H);

	score.init(SCORE_X, SCORE_H, SCORE_W, SCORE_H, 1);
	remainingScore.init(SCORE_X, SCORE_Y, SCORE_W, SCORE_H - SCORE_Y + SCORE, 0);

	idx_beat = 0;
	delayCounter = 0;
	debugMode = false;
	gameEnd = false;
	gameStart = true;
	return true;
}

/**
 * This function correctly releases the resources used in order to free the memory
 */
void Manager::release() {
	SDL_DestroyTexture(background_img);
	SDL_DestroyTexture(left_arrow_btn_img);
	SDL_DestroyTexture(left_arrow_beat_img);
	SDL_DestroyTexture(down_arrow_btn_img);
	SDL_DestroyTexture(down_arrow_beat_img);
	SDL_DestroyTexture(up_arrow_btn_img);
	SDL_DestroyTexture(up_arrow_beat_img);
	SDL_DestroyTexture(right_arrow_btn_img);
	SDL_DestroyTexture(right_arrow_beat_img);
	SDL_DestroyTexture(remainingScore_img);
	SDL_DestroyTexture(score_img);
	SDL_DestroyTexture(start_img);
	IMG_Quit();
	Mix_FreeMusic(bgm);
	Mix_FreeChunk(score_sfx);
	Mix_FreeChunk(fail_sfx);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}

/**
 * This function controls user input
 */
bool Manager::input() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i) {
		if (keyboard[i]) keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}

/**
 * This function manages the events ocurring while playing
 */
bool Manager::update() {

	// Read Input
	if (!input()) return true;

	// Exit the game
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN) return true;

	// Enables debug mode
	if (keys[SDL_SCANCODE_F4] == KEY_DOWN) debugMode = !debugMode;

	// Logic
	// Beats update & input

	delayCounter++;

	if (random(100, 0) <= 10) {

		int randCol = random(4, 1);

		if (!beats[idx_beat].isAlive() && delayCounter >= SPAWN_DELAY) {
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

			beats[idx_beat].init(button.x + SPRITE_W / 2, SPRITE_Y_SPAWN, SPRITE_W, SPRITE_H, SPRITE_SPEED);
			idx_beat++;
			idx_beat %= MAX_BEATS;
			delayCounter = 0;
		}
	}

	for (int i = 0; i < MAX_BEATS; ++i) {
		if (beats[i].isAlive()) {
			beats[i].move(0, 1);
			SDL_Rect button, beat;
			beats[i].getRect(&beat.x, &beat.y, &beat.w, &beat.h);

			// 'A' or 'LEFT KEY' control
			if (keys[SDL_SCANCODE_A] == KEY_DOWN || keys[SDL_SCANCODE_LEFT] == KEY_DOWN) {
				buttonA.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginA)) {
					beats[i].shutDown();
					updateScore(&score, false, fail_sfx);
					break;
				}
			}

			// 'S' or 'DOWN KEY' control
			if (keys[SDL_SCANCODE_S] == KEY_DOWN || keys[SDL_SCANCODE_DOWN] == KEY_DOWN) {
				buttonS.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginS)) {
					beats[i].shutDown();
					updateScore(&score, false, fail_sfx);
					break;
				}
			}

			// 'W' or 'UP KEY' control
			if (keys[SDL_SCANCODE_W] == KEY_DOWN || keys[SDL_SCANCODE_UP] == KEY_DOWN) {
				buttonW.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginW)) {
					beats[i].shutDown();
					updateScore(&score, false, fail_sfx);
					break;
				}
			}

			// 'D' or 'RIGHT KEY' control
			if (keys[SDL_SCANCODE_D] == KEY_DOWN || keys[SDL_SCANCODE_RIGHT] == KEY_DOWN) {
				buttonD.getRect(&button.x, &button.y, &button.w, &button.h);
				if (SDL_HasIntersection(&beat, &error_marginD)) {
					beats[i].shutDown();
					updateScore(&score, false, fail_sfx);
					break;
				}
			}

			// Check if the beats intersect with the button
			if (SDL_HasIntersection(&beat, &button)) {
				beats[i].shutDown();
				updateScore(&score, true, score_sfx);
				break;
			}

			// Check if beats are out of the window
			if (beats[i].getY() >= WINDOW_HEIGHT) {
				beats[i].shutDown();
				updateScore(&score, false, fail_sfx);
			}
		}
	}

	// Closes the game whenever the score reaches the remainingScore
	if (score.getY() <= remainingScore.getY()) {
		gameEnd = true;
		return true;
	}

	return false;
}

/**
 * This function manages all drawing functions
 */
void Manager::draw() {
	// Clears the renderer
	SDL_RenderClear(renderer);

	// Checks if the game has started
	if (gameStart) {
		SDL_RenderCopy(renderer, start_img, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(SCREEN_DELAY);
		gameStart = false;
	}

	// Draw background
	SDL_RenderCopy(renderer, background_img, NULL, NULL);

	SDL_Rect rc;

	if (debugMode) {
		// Draw buttons collisions
		buttonW.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);

		buttonA.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);

		buttonS.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_SetRenderDrawColor(renderer, 50, 50, 0, 255);

		buttonD.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_SetRenderDrawColor(renderer, 0, 0, 50, 255);

		// Draw error margins collisions
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderFillRect(renderer, &error_marginW);
		SDL_RenderFillRect(renderer, &error_marginA);
		SDL_RenderFillRect(renderer, &error_marginS);
		SDL_RenderFillRect(renderer, &error_marginD);

		// Draw beats collisions
		for (int i = 0; i < MAX_BEATS; ++i) {
			if (beats[i].isAlive()) {
				beats[i].getRect(&rc.x, &rc.y, &rc.w, &rc.h);
				if (beats[i].getX() == BUTTON_X + SPRITE_W / 2) {
					SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
				}
				else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN + BUTTON_W + SPRITE_W / 2) {
					SDL_SetRenderDrawColor(renderer, 100, 100, 0, 255);
				}
				else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2 + SPRITE_W / 2) {
					SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
				}
				else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3 + SPRITE_W / 2) {
					SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
				}
				SDL_RenderFillRect(renderer, &rc);
			}
		}
	}

	// Draw buttons
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
			if (beats[i].getX() == BUTTON_X + SPRITE_W / 2) {
				SDL_RenderCopy(renderer, left_arrow_beat_img, NULL, &rc);
			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN + BUTTON_W + SPRITE_W / 2) {
				SDL_RenderCopy(renderer, down_arrow_beat_img, NULL, &rc);
			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 2 + BUTTON_W * 2 + SPRITE_W / 2) {
				SDL_RenderCopy(renderer, up_arrow_beat_img, NULL, &rc);
			}
			else if (beats[i].getX() == BUTTON_X + BUTTON_MARGIN * 3 + BUTTON_W * 3 + SPRITE_W / 2) {
				SDL_RenderCopy(renderer, right_arrow_beat_img, NULL, &rc);
			}
		}
	}

	// Draw score bars
	remainingScore.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, remainingScore_img, NULL, &rc);
	score.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, score_img, NULL, &rc);

	// Checks if the game is ended
	if (gameEnd) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, end_img, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(SCREEN_DELAY);
	}

	// Update screen
	SDL_RenderPresent(renderer);
	SDL_Delay(10);	// 1000/10 = 100 fps max
}
