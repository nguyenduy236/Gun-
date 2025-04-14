#include "bulletObject.h"


bulletObject::bulletObject(SDL_Renderer* renderer, std::string fileName) : baseObject(renderer, fileName){
       _angle = 0;
       _velocity_x = _velocity_y = 0;
       _init_x_in_level = _init_y_in_level = 0;
       _cur_x_in_level = _cur_y_in_level = 0;
       _hit_box = {0, 0, BULLET_SIZE, BULLET_SIZE};
       _damage = 10;
}

bulletObject::~bulletObject(){

}

bool bulletObject::checkMoving() const{

       if(_cur_x_in_level < 0 || _cur_x_in_level > LEVEL_WIDTH) return false;
       if(_cur_y_in_level < 0 || _cur_y_in_level > LEVEL_WIDTH) return false;
       return true;

}

void bulletObject::move(){

       _cur_x_in_level += _velocity_x;
       _cur_y_in_level += _velocity_y;
       updateBox();

}

void bulletObject::draw(SDL_Renderer* renderer, gameMap* Map){

       setRect(_cur_x_in_level - Map->getCurrentCameraX(), _cur_y_in_level - Map->getCurrentCameraY());
       render(renderer);

}

double bulletObject::getDistance() const{
       return (_cur_x_in_level - _init_x_in_level) * (_cur_x_in_level - _init_x_in_level) + (_cur_y_in_level - _init_y_in_level) * (_cur_y_in_level - _init_y_in_level);
}
