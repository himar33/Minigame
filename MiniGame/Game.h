#pragma once

#include "SDL/include/SDL.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL_ttf/include/SDL_ttf.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib")
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib")
#pragma comment( lib, "SDL/libx86/SDL2.lib" )
#pragma comment( lib, "SDL/libx86/SDL2main.lib" )

#include "Entity.h"

#define WINDOW_WIDTH	914
#define WINDOW_HEIGHT	965
#define MAX_KEYS		256
#define MAX_SHOTS		32

class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Release();
	
	bool Input();
	bool Update();
	void Draw();

private:
	SDL_Window *Window;
	SDL_Renderer *Renderer;
	SDL_Surface* surface;

	SDL_Texture* texture_idle;
	SDL_Texture* texture_left;
	SDL_Texture* texture_right;
	SDL_Texture* texture_down;

	SDL_Texture* texture_up;
	SDL_Texture* texture_map;

	SDL_Texture* texture_day;
	SDL_Texture* texture_night;

	SDL_Texture* texture_sun;
	SDL_Texture* texture_moon;

	SDL_Texture* texture_sans;

	SDL_Texture* texture_potato1;
	SDL_Texture* texture_potato2;
	SDL_Texture* texture_potato3;
	SDL_Texture* texture_potato4;
	SDL_Texture* texture_potato5;
	SDL_Texture* texture_potato6;
	SDL_Texture* texture_watering;
	
	SDL_Rect rc;
	Entity Player;
	int idx_shot;
	int png_flag = IMG_INIT_PNG;
	int ogg_flag = MIX_INIT_OGG;
	bool night = false, day = true;
	bool potato[9][7] = { 
	{ true, false, false, false, false, false, false }, { true, false, false, false, false, false, false }, { true, false, false, false, false, false, false },
	{ true, false, false, false, false, false, false }, { true, false, false, false, false, false, false }, { true, false, false, false, false, false, false },
	{ true, false, false, false, false, false, false }, { true, false, false, false, false, false, false }, { true, false, false, false, false, false, false } };
	int life[9] = { 2,2,2,2,2,2,2,2,2 };

	//UI
	TTF_Font* tipografy;
	SDL_Color White;
	SDL_Color Black;
	SDL_Surface* surfaceMessage;
	SDL_Texture* Message;
	SDL_Texture* Message_night;
	SDL_Texture* seeds_night;
	SDL_Texture* seeds_day;
	SDL_Rect Message_rect;
	int recolection = 0;
	int actual = -1;
	int seed_a = -1;
	int seed_r = 0;
	

	bool watering[9] = { false, false, false, false, false, false, false, false, false };

	enum KEY_STATE { KEY_IDLE, KEY_DOWN, KEY_REPEAT, KEY_UP	};
	KEY_STATE keys[MAX_KEYS];
};
