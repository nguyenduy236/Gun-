#include "utils.h"
#include "game.h"

game::game(){

       _mTank = NULL;
       _map = NULL;
       _mouse = NULL;
       _menu = NULL;
       _score_text = NULL;
       _timer_enemy_appear = NULL;
       _gate = NULL;

}

game::~game(){

       freeGameObject();

     if(_menu != NULL){
              delete _menu;
              _menu = NULL;
       }
       if(_mouse != NULL){
              delete _mouse;
              _mouse = NULL;
       }
       if(gMusic != NULL) {
              Mix_FreeMusic(gMusic);
              gMusic = NULL;
       }
       if(_score_text != NULL){
              delete _score_text;
              _score_text = NULL;
       }
       quit();

}

void game::init(int screenWidth, int screenHeight, const char* windowTitle){
       CommonFunction::initSDL(gWindow, gRenderer, screenWidth, screenHeight, windowTitle);

       gMusic = Mix_LoadMUS("data/sound/music_background.wav");
       if(gMusic == NULL) CommonFunction::logSDLError(std::cout, "Loading music background", true);

       _score_text = new Text ("MouldyCheeseRegular-WyMWG.ttf", 30);
       if(_score_text == NULL) CommonFunction::logSDLError(std::cout, "Load score text", true);
       _status = GAME_START;

       _menu = new Menu (gRenderer);
       if(_menu == NULL) CommonFunction::logSDLError(std::cout, "Load menu", true);

       _mouse = new Cursor (gRenderer);
       if(_mouse == NULL) CommonFunction::logSDLError(std::cout, "Load mouse", true);

       srand(time(NULL));
}

void game::freeGameObject(){
       if(_mTank != NULL){
              delete _mTank;
              _mTank = NULL;
       }
       if(_map != NULL){
              delete _map;
              _map = NULL;
       }

       if(_timer_enemy_appear != NULL){
              delete _timer_enemy_appear;
              _timer_enemy_appear = NULL;
       }

       for(enemyTank* eTank : _enemy_tank_list){
              if(eTank != NULL){
                     delete eTank;
                     eTank = NULL;
              }
       }

       if(_gate != NULL){
              delete _gate;
              _gate = NULL;
       }
       _enemy_tank_list.clear();

}

void game::startGame(){

       freeGameObject();

       _map = new gameMap(gRenderer);
       if(_map == NULL) CommonFunction::logSDLError(std::cout, "Load Map", true);

       _gate = new Gate ( gRenderer, "gate.png");
       _gate->setBox( TILE_SIZE * ( MAX_MAP_X - 2 ) + 32 , TILE_SIZE * (MAX_MAP_Y - 2) + 32, 32, 32);

       _score = 0;

       loadMap();

       _mTank = new mainTank (300, MAX_MAIN_HP, 64, 64, gRenderer);
       MAX_ENEMY_HP = 100;
       if(_mTank == NULL) CommonFunction::logSDLError(std::cout, "Load main tank", true);

       for(int _ = ENEMY_NUMBER; _ ; _ --){
              generateEnemyTank();
       }

       _timer_enemy_appear = new LTimer ();
       if(_timer_enemy_appear == NULL) CommonFunction::logSDLError(std::cout, "Load timer enemy appear", true);
       _timer_enemy_appear->start();

}

void game::quit(){
   SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

void game::loadMap(){
       _map->loadTileMap();
}

void game::handleUserInput(SDL_Event e){

       while(SDL_PollEvent(&e)){

              if(e.type == SDL_QUIT){
                     setStatus(GAME_QUIT);
              }

              else if(e.type == SDL_KEYDOWN){
                     if(e.key.keysym.sym == SDLK_ESCAPE){
                            if(getStatus() == GAME_PLAYING) setStatus(GAME_PAUSING);
                     }

              }

              if(getStatus() == GAME_PLAYING){
                   _mTank->handleInputAction(e, gRenderer, _map, _mouse);
              }

              else setStatus( _menu->handleInputAction( e, getStatus(), _mouse ) );

       }

}

void game::handleBulletCollision(){

       std::vector<bulletObject*> main_bullet = _mTank->getBulletList();
       std::vector<bulletObject*> new_main_bullet;

       for(bulletObject* m_bullet : main_bullet){

              bool check = false;

              for(enemyTank* e_tank : _enemy_tank_list){
                     if(CommonFunction::checkCollision(m_bullet->getBox(), e_tank->getBox())){

                            e_tank->changeHealth(-m_bullet->getDamage());
                            e_tank->resetFlicker();

                            _score ++;

                            if(m_bullet != NULL){
                                   delete m_bullet;
                                   m_bullet = NULL;
                            }

                            check = true;
                            break;
                     }
                            }
              if(!check) new_main_bullet.push_back(m_bullet);

       }
       _mTank->setBulletList(new_main_bullet);


       for(enemyTank* e_tank : _enemy_tank_list){

              std::vector<bulletObject*> enemy_bullet = e_tank->getBulletList();
              std::vector<bulletObject*> new_enemy_bullet;

              for(bulletObject* e_bullet : enemy_bullet){

                     if(CommonFunction::checkCollision(e_bullet->getBox(), _mTank->getBox())){

                            _mTank->changeHealth(-e_bullet->getDamage());
                            _mTank->resetFlicker();
                            if(e_bullet != NULL){
                                   delete e_bullet;
                                   e_bullet = NULL;
                            }

                     }
                     else new_enemy_bullet.push_back(e_bullet);

              }
              e_tank->setBulletList(new_enemy_bullet);

       }

}


void game::update(){

       handleMusic();
       handleMouse();
       handleScore();

       if(getStatus() == GAME_PLAYING){

              _mTank->handleKeyState();
              _mTank->updateShortestPath(_map);

              for(auto eTank : _enemy_tank_list){
                     eTank->findPath( _mTank->getShortestPath() );
                     eTank->handleShot(gRenderer, _mTank->getX() + TILE_SIZE / 2, _mTank->getY() + TILE_SIZE / 2);
              }

              handleTankMoving();
              handleBulletCollision();
              _mTank->siteCamera(_map);
              handleLiving();
              handleNextLevel();
              handleEnemyNumber();
       }
       else{
              if(getStatus() == GAME_RESTART){
                     startGame();
                     setStatus(GAME_PLAYING);
              }
              else _menu->update(getStatus());
       }

}

void game::generateEnemyTank(){
       int new_enemy_x = 0, new_enemy_y = 0;

       while(1){

              SDL_Point new_enemy_position = _map->getRandomSoilPos();
              new_enemy_x = new_enemy_position.x;
              new_enemy_y = new_enemy_position.y;

              SDL_Rect new_enemy_rect = {new_enemy_x, new_enemy_y, TANK_SIZE, TANK_SIZE};
              if( CommonFunction::checkCollision(new_enemy_rect, _mTank->getBox() ) ) continue;
              bool isCollideWithOtherTank = false;
              for(enemyTank* eTank : _enemy_tank_list){
                     if(CommonFunction::checkCollision(new_enemy_rect, eTank->getBox()))
                            isCollideWithOtherTank = true;

              }
              if(isCollideWithOtherTank) continue;

                break;
       }
       enemyTank* newETank = new enemyTank (300, MAX_ENEMY_HP, new_enemy_x, new_enemy_y, gRenderer);
       if(newETank == NULL) CommonFunction::logSDLError(std::cout, "Load enemy tank", true);
       _enemy_tank_list.push_back(newETank);
}

void game::handleEnemyNumber(){
       if(_enemy_tank_list.size() < MAX_ENEMY_NUMBER){
              if(_timer_enemy_appear->getTicks() > TIME_ENEMY_APPEAR){
                     generateEnemyTank();
                     _timer_enemy_appear->restart();
              }
       }
       else{
              _timer_enemy_appear->restart();
       }
}

void game::handleTankMoving(){

       int can_main_move = true, check = true;
       _mTank->move();
       if(!_mTank->checkValidPos(_map)) can_main_move = false, _mTank->move_back(), check = false;

       for(int i = 0; i < (int)_enemy_tank_list.size(); i ++){
              enemyTank* eTank = _enemy_tank_list[i];
              eTank->move();
       }

       for(int i = 0; i < (int)_enemy_tank_list.size(); i ++) _enemy_tank_list[i]->move_back();


       std::vector<int> can_enemy_move(_enemy_tank_list.size(), true);

       for(int i = 0; i < (int)_enemy_tank_list.size(); i ++){

              enemyTank* eTank = _enemy_tank_list[i];
              int can_move_if_enemy_stop = !CommonFunction::checkCollision(_mTank->getBox(), eTank->getBox());
              eTank->move();

              if(CommonFunction::checkCollision(_mTank->getBox(), eTank->getBox())){

                     can_main_move &= can_move_if_enemy_stop;
                     can_enemy_move[i] = false;

              }
              if(!eTank->checkValidPos(_map)){
                   can_enemy_move[i] = false;
              }

       }

       if(check && !can_main_move) _mTank->move_back();

       for(int i = 0; i < (int)_enemy_tank_list.size(); i ++){

              if(!can_enemy_move[i]){

                   _enemy_tank_list[i]->move_back();


              }

       }

}




void game::handleEndGame(){

       std::stringstream finalScore;
       finalScore << "Your score: " << _score;

       _menu->setFinalScore(gRenderer, finalScore.str());

       _menu->setAlphaMod(0);
       setStatus(GAME_OVER);

}

void game::handleLiving(){

       if(!_mTank->isLiving()){
              handleEndGame();
       }

       std::vector<enemyTank*> new_enemy_list;

       for(enemyTank* e_tank : _enemy_tank_list){

              if(e_tank == NULL) continue;

              if(! e_tank->isLiving() ){
                     _score += 10;

                     delete e_tank;
                     e_tank = NULL;
              }

              else new_enemy_list.push_back(e_tank);
       }

       _enemy_tank_list = new_enemy_list;

}

void game::handleNextLevel(){
   if(CommonFunction::checkCollision(_mTank->getBox(), _gate->getBox())) {
         handleGoToNextMap();
}
}

void game::handleGoToNextMap(){
    int temp_main_hp = _mTank->getHealth();

       freeGameObject();

       _map = new gameMap(gRenderer);
       if(_map == NULL) CommonFunction::logSDLError(std::cout, "Load Map", true);

       _gate = new Gate ( gRenderer, "gate.png");
       _gate->setBox( TILE_SIZE * ( MAX_MAP_X - 2 ) + 32 , TILE_SIZE * (MAX_MAP_Y - 2) + 32, 32, 32);

       _score += 100;

       loadMap();

       _mTank = new mainTank (300, temp_main_hp + 100, 64, 64, gRenderer);
       MAX_ENEMY_HP += 20;

       if(_mTank == NULL) CommonFunction::logSDLError(std::cout, "Load main tank", true);

       for(int _ = ENEMY_NUMBER; _ ; _ --){
              generateEnemyTank();
       }

       _timer_enemy_appear = new LTimer ();
       if(_timer_enemy_appear == NULL) CommonFunction::logSDLError(std::cout, "Load timer enemy appear", true);
       _timer_enemy_appear->start();
}

void game::handleMusic(){

       if(getStatus() == GAME_PAUSING) Mix_PauseMusic();
       else if(getStatus() == GAME_RESTART) Mix_PlayMusic(gMusic, -1);
       else if(getStatus() == GAME_OVER) Mix_HaltMusic();
       else if(getStatus() == GAME_PLAYING){
       if(Mix_PausedMusic() == 1) Mix_ResumeMusic();
       }

}

void game::handleMouse(){

       _mouse->update();

       if(getStatus() == GAME_PAUSING || getStatus() == GAME_START || getStatus() == GAME_OVER){
              if(_menu->getResume()->isActivated() && CommonFunction::checkCollision(_mouse->getRect(), _menu->getResume()->getRect())) _mouse->setCurrentClip(1);
              else if(_menu->getRestart()->isActivated() && CommonFunction::checkCollision(_mouse->getRect(), _menu->getRestart()->getRect())) _mouse->setCurrentClip(1);
              else if(_menu->getExit()->isActivated() && CommonFunction::checkCollision(_mouse->getRect(), _menu->getExit()->getRect())) _mouse->setCurrentClip(1);
              else _mouse->setCurrentClip(0);
       }

       else _mouse->setCurrentClip(0);

}

void game::handleScore(){

       std::stringstream scoreText;
       scoreText << "Your score: " << _score;
       _score_text->setTextColor({0, 0, 0});
       _score_text->loadText(gRenderer, scoreText.str());

}

void game::renderGamePlay(){

       SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
       SDL_RenderClear(gRenderer);

       if(getStatus() == GAME_PLAYING){

              _map->draw(gRenderer);
              _gate->draw(gRenderer, _map);
              _score_text->draw(gRenderer, 0, 0);

              if(_mTank != NULL && _mTank->isLiving()){
                     _mTank->draw(gRenderer, _map);
                     _mTank->handleBullet(gRenderer, _map);
              }

              for(enemyTank* eTank : _enemy_tank_list){
                     eTank->draw(gRenderer, _map, eTank->getX() + TILE_SIZE / 2, eTank->getY() + TILE_SIZE / 2);
                     eTank->handleBullet(gRenderer, _map);
              }

       }

       if(getStatus() == GAME_PAUSING || getStatus() == GAME_START || getStatus() == GAME_OVER) _menu->draw(gRenderer, getStatus());
       _mouse->draw(gRenderer);

       SDL_RenderPresent(gRenderer);
}
