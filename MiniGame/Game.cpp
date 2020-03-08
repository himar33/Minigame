#include "Game.h"
#include <math.h>

Game::Game() {}
Game::~Game(){}

bool Game::Init()
{
	//Initialize SDL with all subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	//Create our window: title, x, y, w, h, flags
	Window = SDL_CreateWindow("Spaceship: arrow keys + space", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (Window == NULL)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}
	//Create a 2D rendering context for a window: window, device index, flags
	Renderer = SDL_CreateRenderer(Window, -1, 0);
	if (Renderer == NULL)
	{
		SDL_Log("Unable to create rendering context: %s", SDL_GetError());
		return false;
	}
	//Initialize keys array
	for (int i = 0; i < MAX_KEYS; ++i)
		keys[i] = KEY_IDLE;

	//Init variables
	Player.Init(200, 400, NULL, NULL, 3);


	//Start with Audio
	Mix_Init(ogg_flag);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

	//Music

	//Images and surface
	IMG_Init(png_flag);
	surface = IMG_Load("player_idle.png");
	texture_idle = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("player_moveleft.png");
	texture_left = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("player_moveright.png");
	texture_right = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("player_movedown.png");
	texture_down = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("player_moveup.png");
	texture_up = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("mapa.png");
	texture_map = SDL_CreateTextureFromSurface(Renderer, surface);

	return true;
}
void Game::Release()
{
	//Clean up all SDL initialized subsystems
	SDL_Quit();
	SDL_DestroyTexture(texture_idle);
	SDL_DestroyTexture(texture_left);
	SDL_DestroyTexture(texture_right);
	SDL_DestroyTexture(texture_up);
	SDL_DestroyTexture(texture_down);
	SDL_DestroyTexture(texture_map);
	IMG_Quit();
}
bool Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)	return false;
	}

	SDL_PumpEvents();
	const Uint8* keyboard = SDL_GetKeyboardState(NULL);
	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i])
			keys[i] = (keys[i] == KEY_IDLE) ? KEY_DOWN : KEY_REPEAT;
		else
			keys[i] = (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN) ? KEY_UP : KEY_IDLE;
	}

	return true;
}
bool Game::Update()
{
	bool left, right, up, down;

	//Read Input
	if (!Input())  return true;

	//Process Input
	int fx = 0, fy = 0;
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN)	return true;
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT)	fy = -1; up = true;
	if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT)	fy = 1; down = true;
	if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT)	fx = -1; left = true;
	if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT)	fx = 1; right = true;

	//map limits
	if (Player.GetX() > 800 && right == true) {
		fx = -1;
		right = false;
	}

	if (Player.GetX() < 30 && left == true) {
		fx = 1;
		left = false;
	}
	
	if (Player.GetY() < 110 && up == true) {
		fy = 1;
		up = false;
	}

	if (Player.GetY() > 730 && down == true) {
		fy = -1;
		down = false;
	}
	
	//Logic
	//Player update
	Player.Move(fx, fy);
		
	return false;
}
void Game::Draw()
{
	Uint32 ticks = SDL_GetTicks();
	Uint32 seconds = ticks / 1000;
	Uint32 sprite = (ticks / 100) % 4;
	Uint32 sprite_idle = (ticks / 100) % 1;
	SDL_Rect strect = { sprite * 88, 0, 88, 176 };
	SDL_Rect strect_idle = { sprite_idle * 88, 0, 88, 176 };
	SDL_Rect dstrect = { rc.x, rc.y, 88, 176 };
	//Clear rendering target
	SDL_RenderClear(Renderer);

	SDL_SetRenderDrawColor(Renderer, 168, 230, 255, 255);

	//Draw background
	SDL_RenderCopy(Renderer, texture_map, NULL, NULL);

	//Draw player
	
	if (keys[SDL_SCANCODE_UP] == KEY_REPEAT) {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, texture_up, &strect, &dstrect);
	}
	else if (keys[SDL_SCANCODE_DOWN] == KEY_REPEAT) {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, texture_down, &strect, &dstrect);
	}
	else if (keys[SDL_SCANCODE_LEFT] == KEY_REPEAT) {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, texture_left, &strect, &dstrect);
	}
	else if (keys[SDL_SCANCODE_RIGHT] == KEY_REPEAT) {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
		SDL_RenderCopy(Renderer, texture_right, &strect, &dstrect);
	}
	else {
		Player.GetRect(&rc.x, &rc.y, &rc.w, &rc.h);
	SDL_RenderCopy(Renderer, texture_idle, &strect_idle, &dstrect);
	}


	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(16);	// 1000/10 = 100 fps max
}