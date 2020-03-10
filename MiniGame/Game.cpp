#include "Game.h"
#include <math.h>
#include <sstream>

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
	Window = SDL_CreateWindow("Sans Drew Valley", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
	Mix_OpenAudio(20000, MIX_DEFAULT_FORMAT, 2, 1024);

	//Music
	sback = Mix_LoadWAV("fondo.wav");
	Mix_PlayChannel(-1, sback, -1);
	plantar = Mix_LoadWAV("plantar.wav");
	regar = Mix_LoadWAV("regar.wav");
	recoger = Mix_LoadWAV("recoger.wav");
	reloj = Mix_LoadWAV("reloj.wav");
	cash = Mix_LoadWAV("cash.wav");
	buy = Mix_LoadWAV("buy.wav");
	victory = Mix_LoadWAV("victory.wav");

	//Font
	TTF_Init();
	tipografy = TTF_OpenFont("munro.ttf", 18);
	Black = { 0, 0, 0 };
	White = { 255, 255, 255 };


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
	surface = IMG_Load("watering.png");
	texture_watering = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("tienda.png");
	texture_tienda = SDL_CreateTextureFromSurface(Renderer, surface);
	surface = IMG_Load("coin.png");
	texture_coin = SDL_CreateTextureFromSurface(Renderer, surface);

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
	SDL_DestroyTexture(texture_watering);
	SDL_DestroyTexture(texture_tienda);
	SDL_DestroyTexture(texture_coin);
	SDL_DestroyTexture(Message);
	SDL_DestroyTexture(Message_night);
	SDL_DestroyTexture(seeds_day);
	SDL_DestroyTexture(seeds_night);
	SDL_DestroyTexture(m_day);
	SDL_DestroyTexture(m_night);
	Mix_Quit();
	TTF_Quit();
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
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN) {
		if (shop == false)
		{
			return true;
		}
	}
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

	//Recolection update
	std::stringstream p;
	p << "Potatoes: " << recolection;
	std::stringstream s;
	s << "Seeds: " << seed_r;
	std::stringstream m;
	m << m_r;
	if (actual < recolection)
	{
		actual++;
		surfaceMessage = TTF_RenderText_Solid(tipografy, p.str().c_str(), Black);
		Message = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
		surfaceMessage = TTF_RenderText_Solid(tipografy, p.str().c_str(), White);
		Message_night = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
	}
	if (seed_a < seed_r)
	{
		seed_a++;
		surfaceMessage = TTF_RenderText_Solid(tipografy, s.str().c_str(), White);
		seeds_night = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
		surfaceMessage = TTF_RenderText_Solid(tipografy, s.str().c_str(), Black);
		seeds_day = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
	}
	if (m_a < m_r)
	{
		m_a++;
		surfaceMessage = TTF_RenderText_Solid(tipografy, m.str().c_str(), White);
		m_night = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
		surfaceMessage = TTF_RenderText_Solid(tipografy, m.str().c_str(), Black);
		m_day = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
	}

	//Win
	if (win == true)
	{
		int time = 0;
		SDL_Rect win_rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
		SDL_Rect text_rect = { WINDOW_WIDTH/2 - 200,  WINDOW_HEIGHT/2 - 25, 400, 50 };
		std::stringstream t;
		t << "Congratulations, you win!!!";
		surfaceMessage = TTF_RenderText_Solid(tipografy, t.str().c_str(), White);
		text = SDL_CreateTextureFromSurface(Renderer, surfaceMessage);
		while (time < 2000)
		{
			SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(Renderer, &win_rect);
			SDL_RenderCopy(Renderer, text, NULL, &text_rect);
			time++;
			SDL_RenderPresent(Renderer);
		}
		return true;
	}
		
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

	//Define UI
	SDL_Rect Message_rect = { 100, 20, 162, 50 };
	SDL_Rect seeds_rect = { 100, 80, 120, 50 };
	SDL_Rect m_rect = { 840, 30, 20, 50 }; 
	SDL_Rect m_rect2 = { 840, 30, 40, 50 };
	SDL_Rect m_rect3 = { 840, 30, 60, 50 };
	SDL_Rect coin_rect = { 760, 20, 93, 74 };
	SDL_Rect shop_rect = { 157, 178, 600, 600 };

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

	//Key Events
	if (keys[SDL_SCANCODE_SPACE] == KEY_DOWN){

		if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[0][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[0][0] = false;
			potato[0][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[0][0] == false && potato[0][5] == false)
		{
			Mix_PlayChannel(-1, regar, 0);
			watering[0] = true;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[0][5] == true)
		{
			Mix_PlayChannel(-1, recoger, 0);
			potato[0][5] = false;
			potato[0][0] = true;
			recolection++;
			life[0] = 2;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[1][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[1][0] = false;
			potato[1][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[1][0] == false && potato[1][5] == false)
		{
			Mix_PlayChannel(-1, regar, 0);
			watering[1] = true;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[1][5] == true)
		{
			Mix_PlayChannel(-1, recoger, 0);
			potato[1][5] = false;
			potato[1][0] = true;
			recolection++;
			life[1] = 2;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[2][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[2][0] = false;
			potato[2][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[2][0] == false && potato[2][5] == false)
		{
			Mix_PlayChannel(-1, regar, 0);
			watering[2] = true;
		}
		else if (Player.GetY() < 670 - 150 && Player.GetY() > 610 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[2][5] == true)
		{
			Mix_PlayChannel(-1, recoger, 0);
			potato[2][5] = false;
			potato[2][0] = true;
			recolection++;
			life[2] = 2;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[3][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[3][0] = false;
			potato[3][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[3][0] == false && potato[3][5] == false)
		{
			Mix_PlayChannel(-1, regar, 0);
			watering[3] = true;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[3][5] == true)
		{
			Mix_PlayChannel(-1, recoger, 0);
			potato[3][5] = false;
			potato[3][0] = true;
			recolection++;
			life[3] = 2;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[4][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[4][0] = false;
			potato[4][1] = true;
			seed_a -= 2;
			seed_r--;

		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[4][0] == false && potato[4][5] == false)
		{
			Mix_PlayChannel(-1, regar, 0);
			watering[4] = true;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[4][5] == true)
		{
			Mix_PlayChannel(-1, recoger, 0);
			potato[4][5] = false;
			potato[4][0] = true;
			recolection++;
			life[4] = 2;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[5][0] == true && seed_r > 0)
		{
			Mix_PlayChannel(-1, plantar, 0);
			potato[5][0] = false;
			potato[5][1] = true;
			seed_a -= 2;
			seed_r--;

		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[5][0] == false && potato[5][5] == false)
		{
		Mix_PlayChannel(-1, regar, 0);
			watering[5] = true;
		}
		else if (Player.GetY() < 730 - 150 && Player.GetY() > 670 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[5][5] == true)
		{
		Mix_PlayChannel(-1, recoger, 0);
			potato[5][5] = false;
			potato[5][0] = true;
			recolection++;
			life[5] = 2;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[6][0] == true && seed_r > 0)
		{
		Mix_PlayChannel(-1, plantar, 0);
			potato[6][0] = false;
			potato[6][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[6][0] == false && potato[6][5] == false)
		{
		Mix_PlayChannel(-1, regar, 0);
			watering[6] = true;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 610 - 44 && Player.GetX() < 670 - 44 && potato[6][5] == true)
		{
		Mix_PlayChannel(-1, recoger, 0);
			potato[6][5] = false;
			potato[6][0] = true;
			recolection++;
			life[6] = 2;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[7][0] == true && seed_r > 0)
		{
		Mix_PlayChannel(-1, plantar, 0);
			potato[7][0] = false;
			potato[7][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[7][0] == false && potato[7][5] == false)
		{
		Mix_PlayChannel(-1, regar, 0);
			watering[7] = true;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 670 - 44 && Player.GetX() < 730 - 44 && potato[7][5] == true)
		{
		Mix_PlayChannel(-1, recoger, 0);
			potato[7][5] = false;
			potato[7][0] = true;
			recolection++;
			life[7] = 2;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[8][0] == true && seed_r > 0)
		{
		Mix_PlayChannel(-1, plantar, 0);
			potato[8][0] = false;
			potato[8][1] = true;
			seed_a -= 2;
			seed_r--;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[8][0] == false && potato[8][5] == false)
		{
		Mix_PlayChannel(-1, regar, 0);
			watering[8] = true;
		}
		else if (Player.GetY() < 790 - 150 && Player.GetY() > 730 - 150 && Player.GetX() > 730 - 44 && Player.GetX() < 790 - 44 && potato[8][5] == true)
		{
		Mix_PlayChannel(-1, recoger, 0);
			potato[8][5] = false;
			potato[8][0] = true;
			recolection++;
			life[8] = 2;
		}
		else if (Player.GetY() < 500 - 150 && Player.GetY() > 430 - 150 && Player.GetX() > 710 - 44 && Player.GetX() < 780 - 44)
		{
			shop = true;
		}
	}

	//Shop key states
	if (keys[SDL_SCANCODE_ESCAPE] == KEY_DOWN && shop == true)
	{
		shop = false;
	}
	if (keys[SDL_SCANCODE_1] == KEY_DOWN && shop == true)
	{
		if (recolection > 0)	
		{
			Mix_PlayChannel(-1, cash, 0);
			m_r += 4 * recolection;
			actual = -1;
			recolection = 0;
		}
	}
	if (keys[SDL_SCANCODE_3] == KEY_DOWN && shop == true)
	{
		if (m_r >= 1)
		{
			Mix_PlayChannel(-1, buy, 0);
			m_a -= 2;
			seed_r++;
			m_r--;
		}
	}
	if (keys[SDL_SCANCODE_2] == KEY_DOWN && shop == true)
	{
		if (m_r >= 100)
		{
			Mix_PlayChannel(-1, victory, 0);
			m_a -= 101;
			m_r -= 100;
			win = true;
		}
	}

	//Draw watering
	for (int i = 0; i < 9; i++)
	{
		if (watering[i] == true)
		{
			SDL_RenderCopy(Renderer, texture_watering, NULL, &potato_p[i]);
		}
	}

	//Draw potatoes
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
			Mix_PlayChannel(-1, reloj, 0);
			time = 0;
			while (time < 1600)
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
			Mix_PlayChannel(-1, reloj, 0);
			time = 0;
			while (time < 1600)
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
				if (watering[i] == true)
				{
					life[i]++;
				}
				life[i]--;
				watering[i] = false;
			}
			else if (potato[i][2] == true)
			{
				potato[i][2] = false;
				potato[i][3] = true;
				if (watering[i] == true)
				{
					life[i]++;
				}
				life[i]--;
				watering[i] = false;
			}
			else if (potato[i][3] == true)
			{
				potato[i][3] = false;
				potato[i][4] = true;
				if (watering[i] == true)
				{
					life[i]++;
				}
				life[i]--;
				watering[i] = false;
			}
			else if (potato[i][4] == true)
			{
				potato[i][4] = false;
				potato[i][5] = true;
				if (watering[i] == true)
				{
					life[i]++;
				}
				life[i]--;
				watering[i] = false;
			}
			else if (potato[i][5] == true)
			{
				if (watering[i] == true)
				{
					life[i]++;
				}
				life[i]--;
				watering[i] = false;
			}
		}
	}

	//Interface

	//Draw shop
	if (shop == true)
	{
		SDL_RenderCopy(Renderer, texture_tienda, NULL, &shop_rect);
	}

	if (day == true)
	{
		SDL_RenderCopy(Renderer, Message, NULL, &Message_rect);
		SDL_RenderCopy(Renderer, seeds_day, NULL, &seeds_rect);
		if (m_r >= 10 && m_r < 100)
		{
			SDL_RenderCopy(Renderer, m_day, NULL, &m_rect2);
		}
		else if (m_r >= 100)
		{
			SDL_RenderCopy(Renderer, m_day, NULL, &m_rect3);
		}
		else
		{
			SDL_RenderCopy(Renderer, m_day, NULL, &m_rect);
		}
	}
	if (night == true)
	{
		SDL_RenderCopy(Renderer, Message_night, NULL, &Message_rect);
		SDL_RenderCopy(Renderer, seeds_night, NULL, &seeds_rect);
		if (m_r >= 10 && m_r < 100)
		{
			SDL_RenderCopy(Renderer, m_night, NULL, &m_rect2);
		}
		else if (m_r >= 100)
		{
			SDL_RenderCopy(Renderer, m_night, NULL, &m_rect3);
		}
		else
		{
			SDL_RenderCopy(Renderer, m_night, NULL, &m_rect);
		}
	}

	//COIN UI
	SDL_RenderCopy(Renderer, texture_coin, NULL, &coin_rect);

	//Update screen
	SDL_RenderPresent(Renderer);

	SDL_Delay(16);	// 1000/10 = 100 fps max
}