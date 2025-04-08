#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<iostream>
#include<string>
#include<vector>
#include<cstdio>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include<algorithm>
#include<utility>
#include<queue>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const char WINDOW_TITLE[] = "GUN";

const int BOARD_WIDTH = 45;
const int BOARD_HEIGHT = 30;

const int TILE_SIZE = 64;
const int TANK_SIZE = 56;
const int MAX_MAP_X = 30;
const int MAX_MAP_Y = 15;
const int MAIN_TANK_VELOCITY = 4;
const int ENEMY_TANK_VELOCITY = 4;

const int MAIN_BULLET_VELOCITY = 8;
const int ENEMY_BULLET_VELOCITY = 8;
const int BULLET_SIZE = 15;


const int LEVEL_WIDTH = MAX_MAP_X * TILE_SIZE;
const int LEVEL_HEIGHT = MAX_MAP_Y * TILE_SIZE;
const int COLOR_KEY_R = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_B = 255;
const int ENEMY_ATTACK_SPEED = 200;
const int ENEMY_NUMBER =2;
const int MAX_ENEMY_NUMBER = 3;
const int FRAME_FLICKER = 4;
const int TIME_ENEMY_APPEAR = 3000;
const int HP_WIDTH = 64;
const int HP_HEIGHT = 10;
const int MAX_MAIN_HP = 700;
static int MAX_ENEMY_HP = 100;


const SDL_Color MAIN_HP_COLOR = {34, 177, 76, 255}; // GREEN
const SDL_Color ENEMY_HP_COLOR = {237, 28, 36, 255}; // RED

enum TILE_TYPE{
       SOIL,
       WATER,
       GRASS,
       TOTAL_TILE_TYPE
};

enum directState{
       UP,
       DOWN,
       LEFT,
       RIGHT,
       TOTAL_DIRECTION
};

enum team{
       GREEN,
       RED
};

enum gameStatus{
       GAME_START,
       GAME_RESTART,
       GAME_PLAYING,
       GAME_PAUSING,
       GAME_OVER,
       GAME_QUIT
};

namespace CommonFunction{
       void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, int screenWidth, int screenHeight, const char* windowTitle);

       void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);

       bool checkCollision(const SDL_Rect& object1, const SDL_Rect& object2);

       bool checkPointInsideRect(const int& x, const int& y, const SDL_Rect& rect);
}

#endif // UTILS_H_INCLUDED
