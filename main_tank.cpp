#include "main_tank.h"

mainTank::mainTank(int iAttackRange, int iHealthPoint, int iX, int iY, SDL_Renderer* renderer) : baseObject(renderer, "mainTank.png"){
       _attack_range = iAttackRange;
       _health_point = _max_health_point = iHealthPoint;

       _x_in_level = iX;
       _y_in_level = iY;

       _hit_box = {_x_in_level + 4, _y_in_level + 4, TANK_SIZE, TANK_SIZE};

       _current_direction = UP;

       _gun = new gunTank (renderer, "gunTank.png");
       if(_gun == NULL) CommonFunction::logSDLError(std::cout, "Load gun", true);

       _shot_sound = Mix_LoadWAV("data/sound/shot_sound.wav");
       if(_shot_sound == NULL) CommonFunction::logSDLError(std::cout, "Load shot_sound", true);

       _shortest_path = new int *[MAX_MAP_Y];
       for(int i = 0; i < MAX_MAP_Y; i ++){
              _shortest_path[i] = new int [MAX_MAP_X];
       }

}

mainTank::~mainTank(){
       if(_gun != NULL){
              delete _gun;
              _gun = NULL;
       }

       if(_shot_sound != NULL){
           Mix_FreeChunk(_shot_sound);
           _shot_sound = NULL;
       }

       _velocity_x = _velocity_y = 0;
       _attack_range = 0;
       _x_in_level = _y_in_level = 0;
       _current_direction = UP;
       _health_point = 0;

       for(bulletObject* bullet : _bullet_list){
              if(bullet != NULL){
                     delete bullet;
                     bullet = NULL;
              }
       }

       _bullet_list.clear();

       for(int i = 0; i < MAX_MAP_Y; i ++){
              delete[] _shortest_path[i];
       }
       delete[] _shortest_path;
}

void mainTank::flicker(){
       if(_i_flicker < FRAME_FLICKER) setAlpha(64), _i_flicker ++;
       else setAlpha(255);
}

void mainTank::draw(SDL_Renderer* renderer, gameMap* camera){

       setRect(getX() - camera->getCurrentCameraX(), getY() - camera->getCurrentCameraY());
       flicker();

       switch(_current_direction){
              case UP: render(renderer, NULL, 0, NULL, SDL_FLIP_NONE); break;
              case RIGHT: render(renderer, NULL, 90, NULL, SDL_FLIP_NONE); break;
              case DOWN: render(renderer, NULL, 180, NULL, SDL_FLIP_NONE); break;
              case LEFT: render(renderer, NULL, 270, NULL, SDL_FLIP_NONE); break;
              default: break;
       }

       _gun->setRect(getRect().x, getRect().y);

       SDL_Point _gunCenter = {_gun->getRect().x + _gun->getRect().w / 2, getRect().y + getRect().h / 2};
       int mouseX = 0, mouseY = 0;
       SDL_GetMouseState(&mouseX, &mouseY);

       _gun->setAngle( atan2(mouseY - _gunCenter.y, mouseX - _gunCenter.x) / M_PI * 180 + 90 );
       _gun->draw(renderer);

       renderHPBar(renderer, getRect().x, getRect().y + TILE_SIZE + 5, HP_WIDTH, HP_HEIGHT, MAIN_HP_COLOR);
}

void mainTank::handleInputAction(const SDL_Event& e, SDL_Renderer* renderer, gameMap* Map, Cursor* mouse){

       if(e.type == SDL_MOUSEBUTTONDOWN){

              if(e.button.button == SDL_BUTTON_LEFT){

                     int mouseX = mouse->getMouseX(), mouseY = mouse->getMouseY();
                     mouseX += Map->getCurrentCameraX();
                     mouseY += Map->getCurrentCameraY();

                     double X_center = getX() + TILE_SIZE * 0.5, Y_center = getY() + TILE_SIZE * 0.5;

                     bulletObject *newBullet = new bulletObject(renderer, "bullet.png");
                     newBullet->setAngle(atan2(mouseY - Y_center, mouseX - X_center));
                     newBullet->setInitialPosition( X_center + TILE_SIZE * cos( newBullet->getAngle() ) * 0.5 - BULLET_SIZE * 0.5, Y_center + TILE_SIZE * sin( newBullet->getAngle() ) * 0.5 - BULLET_SIZE * 0.5);
                     newBullet->setVelocity( MAIN_BULLET_VELOCITY * cos( newBullet->getAngle() ), MAIN_BULLET_VELOCITY * sin( newBullet -> getAngle() ) );
                     newBullet->updateBox();

                     Mix_PlayChannel(-1, _shot_sound, 0);

                     _bullet_list.push_back(newBullet);

              }
       }
}

void mainTank::handleKeyState(){
       const Uint8* keyStates = SDL_GetKeyboardState(NULL);

       if(keyStates[SDL_SCANCODE_A]) _velocity_x = -MAIN_TANK_VELOCITY, _current_direction = LEFT;
       else if(keyStates[SDL_SCANCODE_D]) _velocity_x = MAIN_TANK_VELOCITY, _current_direction = RIGHT;
       else _velocity_x = 0;

       if(keyStates[SDL_SCANCODE_W]) _velocity_y = -MAIN_TANK_VELOCITY, _current_direction = UP;
       else if(keyStates[SDL_SCANCODE_S]) _velocity_y = MAIN_TANK_VELOCITY, _current_direction = DOWN;
       else _velocity_y = 0;


}
bool mainTank::checkValidPos(gameMap *Map) const{
   if(_x_in_level < 0 || _x_in_level + TANK_SIZE > LEVEL_WIDTH) return false;
   if(_y_in_level < 0 || _y_in_level + TANK_SIZE > LEVEL_HEIGHT) return false;

   int _x_in_tileMap = 0, _y_in_tileMap = 0;

   //check trái trên
   _x_in_tileMap = getBox().x/TILE_SIZE;
   _y_in_tileMap = getBox().y/TILE_SIZE;
   if(Map->getTileType(_x_in_tileMap, _y_in_tileMap) == STONE || Map->getTileType(_x_in_tileMap, _y_in_tileMap) == ICE) return false;

   // check trai duoi
   _x_in_tileMap = getBox().x/TILE_SIZE;
   _y_in_tileMap = ((getBox().y + getBox().h-1))/TILE_SIZE ;
   if(Map->getTileType(_x_in_tileMap, _y_in_tileMap) == STONE || Map->getTileType(_x_in_tileMap, _y_in_tileMap) == ICE) return false;

   // check phải trên
   _x_in_tileMap = (getBox().x + getBox().w - 1)/TILE_SIZE;
   _y_in_tileMap = getBox().y/TILE_SIZE;
   if(Map->getTileType(_x_in_tileMap, _y_in_tileMap) == STONE || Map->getTileType(_x_in_tileMap, _y_in_tileMap) == ICE) return false;

   // check dưới phải
    _x_in_tileMap = (getBox().x + getBox().w - 1)/TILE_SIZE;
   _y_in_tileMap = (getBox().y + getBox().h - 1)/TILE_SIZE;
   if(Map->getTileType(_x_in_tileMap, _y_in_tileMap) == STONE || Map->getTileType(_x_in_tileMap, _y_in_tileMap) == ICE) return false;

    return true;
}

void mainTank::handleBullet(SDL_Renderer* renderer, gameMap* Map){
       std::vector<bulletObject*> new_bullet_list;
       for(int i = 0; i < (int)_bullet_list.size(); i ++){

              bulletObject* p_bullet = _bullet_list[i];

              if(p_bullet->checkMoving() && p_bullet->getDistance() <= _attack_range * 1ll * _attack_range){
                     new_bullet_list.push_back(p_bullet);
                     p_bullet->draw(renderer, Map);
                     p_bullet->move();
              }

              else{

                     if(p_bullet != NULL){
                            delete p_bullet;
                            p_bullet = NULL;
                     }

              }

       }

       _bullet_list = new_bullet_list;

}

void mainTank::move(){
       _x_in_level += _velocity_x;
       _y_in_level += _velocity_y;
       updateBox();
}

void mainTank::move_back(){
       _x_in_level -= _velocity_x;
       _y_in_level -= _velocity_y;
       updateBox();
}

void mainTank::siteCamera(gameMap* camera){

       int curX = _x_in_level + getRect().w / 2 - SCREEN_WIDTH / 2;
       if(curX < 0) curX = 0;
       if(curX + SCREEN_WIDTH > LEVEL_WIDTH) curX = LEVEL_WIDTH - SCREEN_WIDTH;

       int curY = _y_in_level + getRect().h / 2 - SCREEN_HEIGHT / 2;
       if(curY < 0) curY = 0;
       if(curY + SCREEN_HEIGHT > LEVEL_HEIGHT) curY = LEVEL_HEIGHT - SCREEN_HEIGHT;

       camera->setCurrentCamera(curX, curY);
}



void mainTank::renderHPBar(SDL_Renderer* renderer, const int& x, const int& y, const int &w, const int& h, SDL_Color color){

       SDL_Color oldColor;
       SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
       SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // init full black rect

       SDL_Rect HPRect = {x, y, w, h};
       SDL_RenderFillRect(renderer, &HPRect);

       SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // draw green rect
       SDL_Rect currentHPRect = {x + 2, y + 2, w - 4, h - 4};
       currentHPRect.w *= 1.0 * _health_point / _max_health_point;
       SDL_RenderFillRect(renderer, &currentHPRect);

       SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

bool mainTank::fitToCell(){
       int x = getBox().x / TILE_SIZE,
           y = getBox().y / TILE_SIZE;
       return getBox().x + getBox().w <= TILE_SIZE * (x + 1) &&
              getBox().y + getBox().h <= TILE_SIZE * (y + 1);
}

void mainTank::updateShortestPath(gameMap* Map){
       if(fitToCell()){

              for(int i = 0; i < MAX_MAP_Y; i ++){

                     for(int j = 0; j < MAX_MAP_X; j ++){

                            _shortest_path[i][j] = -1;

                     }

              }

              static int directionX[] = {1, -1, 0, 0};
              static int directionY[] = {0, 0, 1, -1};

              std::queue<std::pair<int, int>> cell_queue;
              int sx = getBox().x / TILE_SIZE,
                  sy = getBox().y / TILE_SIZE;

              _shortest_path[sy][sx] = 0;
              cell_queue.push( { sx , sy } );

              while( !cell_queue.empty() ){

                     int x = cell_queue.front().first, y = cell_queue.front().second;
                     cell_queue.pop();

                     for(int i = 0; i < 4; i ++){

                            int nx = x + directionX[i],
                                ny = y + directionY[i];

                            if( nx >= 0 && nx < MAX_MAP_X && ny >= 0 && ny < MAX_MAP_Y ){

                                   if(Map->getTileType(nx, ny) == SOIL){

                                          if(_shortest_path[ny][nx] == -1){

                                                 _shortest_path[ny][nx] = _shortest_path[y][x] + 1;
                                                 cell_queue.push({ nx, ny });

                                          }

                                   }

                            }
                     }
              }
       }
}
