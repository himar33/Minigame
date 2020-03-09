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
	surface = IMG_Load("day.png");
	texture_day = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("night.png");
	texture_night = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("sun.png");
	texture_sun = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("moon.png");
	texture_moon = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("sans.png");
	texture_sans = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_1.png");
	texture_potato1 = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_2.png");
	texture_potato2 = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_3.png");
	texture_potato3 = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_4.png");
	texture_potato4 = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_5.png");
	texture_potato5 = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("Patata_6.png");
	texture_potato6 = SDL_CreateTextureFromSurface(Renderer, surface);

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
	SDL_DestroyTexture(texture_day);
	SDL_DestroyTexture(texture_night);
	SDL_DestroyTexture(texture_potato1);
	SDL_DestroyTexture(texture_potato2);
	SDL_DestroyTexture(texture_potato3);
	SDL_DestroyTexture(texture_potato4);
	SDL_DestroyTexture(texture_potato5);
	SDL_DestroyTexture(texture_potato6);
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
	bool left = false, right = false, up = false, down = false;

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
	else if (Player.GetX() < 30 && left == true) {
		fx = 1;
		left = false;
	}
	else if (Player.GetY() < 110 && up == true) {
		fy = 1;
		up = false;
	}
	else if (Player.GetY() > 730 && down == true) {
		fy = -1;
		down = false;
	}

	//box limit
	if (Player.GetY() < 315 && Player.GetY() > 305 && Player.GetX() < 415 && Player.GetX() > 275 && up == true) {
		fy = 1;
	}
	else if (Player.GetY() < 210 && Player.GetY() > 200 && Player.GetX() < 415 && Player.GetX() > 275 && down == true) {
		fy = -1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 200 && Player.GetX() < 425 && Player.GetX() > 415 && left== true) {
		fx = 1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 200 && Player.GetX() > 265 && Player.GetX() < 275 && right == true) {
		fx = -1;
	}

	//tent camping limit
	if (Player.GetY() < 315 && Player.GetY() > 305 && Player.GetX() < 260 && Player.GetX() > 75 && up == true) {
		fy = 1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 110 && Player.GetX() < 260 && Player.GetX() > 250 && left == true) {
		fx = 1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 110 && Player.GetX() > 75 && Player.GetX() < 85 && right == true) {
		fx = -1;
	}

	//house tent limit
	if (Player.GetY() < 315 && Player.GetY() > 305 && Player.GetX() < 790 && Player.GetX() > 432 && up == true) {
		fy = 1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 110 && Player.GetX() < 790 && Player.GetX() > 780 && left == true) {
		fx = 1;
	}
	else if (Player.GetY() < 315 && Player.GetY() > 110 && Player.GetX() > 432 && Player.GetX() < 442 && right == true) {
		fx = -1;
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
	SDL_Rect day_p = { 0, 0, 1024, 334 };
	SDL_Rect night_p = { 0, 0, 1200, 548 };
	SDL_Rect sun_p = { 20, 20, 50, 50 };
	SDL_Rect moon_p = { 20, 20, 50, 50 };
	SDL_Rect transition = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	SDL_Rect sans_p = { WINDOW_WIDTH/2 - 240, WINDOW_HEIGHT/2 - 195, 480, 390 };
	SDL_Rect plant = { 610, 610, 185, 185 };
	SDL_Rect potato_p[9] = 
	{ 
		{610, 610, 60, 60}, {670, 610, 60, 60}, {730, 610, 60, 60},
	    {610, 670, 60, 60}, {670, 670, 60, 60}, {730, 670, 60, 60},
	    {610, 730, 60, 60}, {670, 730, 60, 60}, {730, 730, 60, 60} 
	};

	int time = 0;
	
	//Clear rendering target
	SDL_RenderClear(Renderer);

	//Time Condition
	if (day == true)
	{
		SDL_RenderCopy(Renderer, texture_day, NULL, &day_p);
		SDL_RenderCopy(Renderer, texture_sun, NULL, &sun_p);
	}
	else if (night == true)
	{
		SDL_RenderCopy(Renderer, texture_night, NULL, &night_p);
		SDL_RenderCopy(Renderer, texture_moon, NULL, &moon_p);
	}

	//Draw background
	SDL_RenderCopy(Renderer, texture_map, NULL, NULL);

	//Draw potato
	for (int i = 0; i < 9; i++)
	{
		if (life[i] <= 0)
		{
			potato[i][0] = true;
			potato[i][1] = false;
			potato[i][2] = false;
			potato[i][3] = false;
			potato[i][4] = false;
			potato[i][5] = false;
			life[i] = 2;
		}
	}

	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN){

		if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[0][0] == true)
		{
			potato[0][0] = false;
			potato[0][1] = true;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[1][0] == true)
		{
			potato[1][0] = false;
			potato[1][1] = true;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[2][0] == true)
		{
			potato[2][0] = false;
			potato[2][1] = true;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[3][0] == true)
		{
			potato[3][0] = false;
			potato[3][1] = true;

		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[4][0] == true)
		{
			potato[4][0] = false;
			potato[4][1] = true;

		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[5][0] == true)
		{
			potato[5][0] = false;
			potato[5][1] = true;

		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[6][0] == true)
		{
			potato[6][0] = false;
			potato[6][1] = true;

		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[7][0] == true)
		{
			potato[7][0] = false;
			potato[7][1] = true;

		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[8][0] == true)
		{
			potato[8][0] = false;
			potato[8][1] = true;

		}
	}

	for (int i = 0; i < 9; i++)
	{
		if (potato[i][1] == true && life[i] > 0) {
			SDL_RenderCopy(Renderer, texture_potato1, NULL, &potato_p[i]);
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (potato[i][2] == true && life[i] > 0) {
			SDL_RenderCopy(Renderer, texture_potato2, NULL, &potato_p[i]);
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (potato[i][3] == true && life[i] > 0) {
			SDL_RenderCopy(Renderer, texture_potato3, NULL, &potato_p[i]);
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (potato[i][4] == true && life[i] > 0) {
			SDL_RenderCopy(Renderer, texture_potato4, NULL, &potato_p[i]);
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (potato[i][5] == true && life[i] > 0) {
			SDL_RenderCopy(Renderer, texture_potato5, NULL, &potato_p[i]);
		}
	}


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

	//Time Changer
	//SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN && Player.GetY() < 357 && Player.GetY() > 315 && Player.GetX() > 137 && Player.GetX() < 199) {
		if (day == true)
		{
			time = 0;
			while (time < 1500)
			{
				SDL_RenderFillRect(Renderer, &transition);
				SDL_RenderCopy(Renderer, texture_sans, NULL, &sans_p);
				time++;
				SDL_RenderPresent(Renderer);
			}
			day = false;
			night = true;
		}
		else if (night == true)
		{
			time = 0;
			while (time < 1500)
			{
				SDL_RenderFillRect(Renderer, &transition);
				SDL_RenderCopy(Renderer, texture_sans, NULL, &sans_p);
				time++;
				SDL_RenderPresent(Renderer);
			}
			night = false;
			day = true;
		}
		for (int i = 0; i < 9; i++)
		{
			if (potato[i][1] == true)
			{
				potato[i][1] = false;
				potato[i][2] = true;
				life[i]--;
			}
			else if (potato[i][2] == true)
			{
				potato[i][2] = false;
				potato[i][3] = true;
				life[i]--;
			}
			else if (potato[i][3] == true)
			{
				potato[i][3] = false;
				potato[i][4] = true;
				life[i]--;
			}
			else if (potato[i][4] == true)
			{
				potato[i][4] = false;
				potato[i][5] = true;
				life[i]--;
			}
		}

	}


	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(16);	// 1000/10 = 100 fps max
}