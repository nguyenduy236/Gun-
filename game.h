#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "utils.h"
#include "main_tank.h"
#include "game_map.h"
#include "enemy_tank.h"
#include "menu.h"
#include "cursor.h"
#include "text.h"
#include "Gate.h"
#include "timers.h"
class game{
public:
       game();
       ~game();

       void init(int screenWidth, int screenHeight, const char* windowTitle);
       void quit();
       void handleUserInput(SDL_Event e);
       void renderGamePlay();
       void loadMap();
       void update();
       bool isRunning() const {return _status != GAME_QUIT;}
       void setStatus(gameStatus gS){_status = gS;}
       void generateEnemyTank();
       gameStatus getStatus() const{return _status;}
       void handleTankMoving();
       void handleBulletCollision();
       void handleLiving();
       void startGame();
       void handleMusic();
       void handleMouse();
       void handleEnemyNumber();
       void handleScore();
       void handleEndGame();
       void freeGameObject();
       std::vector<enemyTank*> getEnemyTankList() const {return _enemy_tank_list;}
       void getMouseState(int *x, int *y){*x = _mouse->getMouseX(), *y = _mouse->getMouseY();}
       mainTank* getMainTank() const {return _mTank;}

private:

       SDL_Window* gWindow;
       SDL_Renderer* gRenderer;
       Mix_Music* gMusic;

       int _score, _highest_score;
       gameStatus _status;
       mainTank *_mTank;
       gameMap *_map;
       Gate* _gate;

       std::vector<enemyTank*> _enemy_tank_list;

       Menu* _menu;
       Cursor* _mouse;
       Text* _score_text;
       LTimer* _timer_enemy_appear;

};

#endif // GAME_H_INCLUDED
