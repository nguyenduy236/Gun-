#include "enemy_tank.h"

enemyTank::enemyTank(int iAttackRange, int iHealthPoint, int iX, int iY, SDL_Renderer* renderer) : baseObject(renderer, "enemyTankBase.png"){

       _attack_range = iAttackRange;
       _health_point = _max_health_point = iHealthPoint;
       _current_direction = UP;
       _x_in_level = iX;
       _y_in_level = iY;
       _hit_box = {_x_in_level + 4, _y_in_level + 4, TANK_SIZE, TANK_SIZE};
       _i_flicker = 0;

       _gun = new gunTank (renderer, "enemyTankTurret.png");
       if(_gun == NULL) CommonFunction::logSDLError(std::cout, "Load enemy gun", true);

       _timer_shot = new LTimer();
       if(_timer_shot == NULL) CommonFunction::logSDLError(std::cout, "Load timer enemy shot", true);

       _timer_shot->start();

}

enemyTank::~enemyTank(){

       if(_gun != NULL){
              delete _gun;
              _gun = NULL;
       }

       for(bulletObject* bullet : _bullet_list){
              if(bullet != NULL){
                     delete bullet;
                     bullet = NULL;
              }
       }

       if(_timer_shot != NULL){
              delete _timer_shot;
              _timer_shot = NULL;
       }

       _bullet_list.clear();
}

void enemyTank::flicker(){

       if(_i_flicker < FRAME_FLICKER) setAlpha(128), _i_flicker ++;
       else setAlpha(255);

}

void enemyTank::draw(SDL_Renderer* renderer, gameMap* camera, const int& main_tank_x, const int& main_tank_y){

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
       _gun->draw(renderer);

       renderHPBar(renderer, getRect().x, getRect().y + TILE_SIZE + 5, HP_WIDTH, HP_HEIGHT, ENEMY_HP_COLOR);

}
bool enemyTank::checkValidPos(gameMap *Map) const{
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

void enemyTank::handleShot(SDL_Renderer* renderer, const int& pos_x, const int &pos_y){
       double distance = sqrt((pos_x - _x_in_level) * (pos_x - _x_in_level) + (pos_y - _y_in_level) * (pos_y - _y_in_level));

       if(distance <= _attack_range + 100){

              if(_timer_shot->getTicks() >= ENEMY_ATTACK_SPEED){
                     bulletObject *newBullet = new bulletObject(renderer, "enemyBullet.png");

                     double X_center = getX() + TILE_SIZE * 0.5, Y_center = getY() + TILE_SIZE * 0.5;
                     double angle = atan2(pos_y - Y_center, pos_x - X_center) + (-400 + rand() % 800) * 0.001;

                     _gun->setAngle(angle * 180 / M_PI + 90);

                     newBullet->setAngle(angle);
                     newBullet->setInitialPosition( X_center + TILE_SIZE * cos( newBullet->getAngle() ) * 0.5 - BULLET_SIZE * 0.5, Y_center + TILE_SIZE * sin( newBullet->getAngle() ) * 0.5 - BULLET_SIZE * 0.5);
                     newBullet->setVelocity( ENEMY_BULLET_VELOCITY * cos( newBullet->getAngle() ), ENEMY_BULLET_VELOCITY * sin( newBullet -> getAngle() ) );
                     newBullet->updateBox();

                     _bullet_list.push_back(newBullet);

                     _timer_shot->restart();
              }

       }
}

void enemyTank::handleBullet(SDL_Renderer* renderer, gameMap* Map){
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

void enemyTank::findPath(int **shortest_path){

       if(fitToCell()){

              static int directionX[] = {1, -1, 0, 0};
              static int directionY[] = {0, 0, 1, -1};
              static directState direction[] = {RIGHT, LEFT, DOWN, UP};

              int x = getBox().x / TILE_SIZE, y = getBox().y / TILE_SIZE;

              for(int i = 0; i < 4; i ++){

                     int nx = x + directionX[i], ny = y + directionY[i];
                     if(shortest_path[ny][nx] + 1 == shortest_path[y][x]){

                            _current_direction = direction[i];
                            _velocity_x = directionX[i] * ENEMY_TANK_VELOCITY;
                            _velocity_y = directionY[i] * ENEMY_TANK_VELOCITY;

                            return;
                     }

              }
       }

}

void enemyTank::move(){
       _x_in_level += _velocity_x;
       _y_in_level += _velocity_y;
       updateBox();
}

void enemyTank::move_back(){
       _x_in_level -= _velocity_x;
       _y_in_level -= _velocity_y;
       updateBox();
}


void enemyTank::renderHPBar(SDL_Renderer* renderer, const int& x, const int& y, const int &w, const int& h, SDL_Color color){
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

bool enemyTank::fitToCell(){

       int x = getBox().x / TILE_SIZE,
           y = getBox().y / TILE_SIZE;

       return (getBox().x + getBox().w - 1) / TILE_SIZE == x &&
              (getBox().y + getBox().h - 1) / TILE_SIZE == y;

}

