#include "manager.h"
#include <math.h>

Manager::Manager() {}
Manager::~Manager() {}

bool Manager::init() {
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//Create our window: title, x, y, w, h, flags
	window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	//Create a 2D rendering context for a window: window, device index, flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}

	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i) {
		keys[i] = KEY_IDLE;
	}

	if (!loadTextures()) {
		return false;
	}

	//Init variables
	player.init(20, WINDOW_HEIGHT >> 1, 104, 82, 5);
	idx_shot = 0;

	return true;
}

bool Manager::loadTextures() {
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		SDL_Log("IMG_Init: Failed to init required jpg and png support!\n");
		SDL_Log("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}
	background_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("background.png"));

	if (background_img == NULL) {
		SDL_Log("Couldn't load texture for background_img: %s", SDL_GetError());
		return false;
	}

	player_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("spaceship.png"));
	if (player_img == NULL) {
		SDL_Log("Couldn't load texture for player_img: %s", SDL_GetError());
		return false;
	}
	
	shot_img = SDL_CreateTextureFromSurface(renderer, IMG_Load("shot.png"));
	if (shot_img == NULL) {
		SDL_Log("Couldn't load texture for shot_img: %s", SDL_GetError());
		return false;
	}
	SDL_Surface* surface = IMG_Load("shot.png");
	return true;
}

void Manager::release() {
	//Clean up all SDL initialized subsystems
	SDL_DestroyTexture(background_img);
	SDL_DestroyTexture(player_img);
	SDL_DestroyTexture(shot_img);
	IMG_Quit();
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
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}

bool Manager::update() {
	//Read Input
	if (!input()) {
		return true;
	}

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1;
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN) {
		int x, y, w, h;
		player.getRect(&x, &y, &w, &h);
		Shots[idx_shot].init(x + 29, y + 3, 56, 20, 10);

		idx_shot++;
		Shots[idx_shot].init(x + 29, y + 59, 56, 20, 10);

		idx_shot++;
		idx_shot %= MAX_SHOTS;
	}

	//Logic
	//Player update
	player.move(fx, fy);
	//Shots update
	for (int i = 0; i < MAX_SHOTS; ++i) {
		if (Shots[i].isAlive()) {
			Shots[i].move(1, 0);
			if (Shots[i].getX() > WINDOW_WIDTH)	Shots[i].shutDown();
		}
	}

	return false;
}

void Manager::draw() {
	SDL_RenderClear(renderer);
	//Set the color used for drawing operations
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//Clear rendering target
	//SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background_img, NULL, NULL);
	//Draw player
	SDL_Rect rc;
	player.getRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(renderer, player_img, NULL, &rc);
	//SDL_RenderFillRect(renderer, &rc);

	//Draw shots
	SDL_SetRenderDrawColor(renderer, 192, 0, 0, 255);
	for (int i = 0; i < MAX_SHOTS; ++i) {
		if (Shots[i].isAlive()) {
			Shots[i].getRect(&rc.x, &rc.y, &rc.w, &rc.h);
			SDL_RenderCopy(renderer, shot_img, NULL, &rc);
			//SDL_RenderFillRect(renderer, &rc);
		}
	}

	//Update screen
	SDL_RenderPresent(renderer);
	
	SDL_Delay(10);	// 1000/10 = 100 fps max
}
