#ifndef BULLETOBJECT_H_INCLUDED
#define BULLETOBJECT_H_INCLUDED

#include "utils.h"
#include "game_map.h"
#include "base_object.h"

class bulletObject : public baseObject{
public:
       bulletObject(SDL_Renderer* renderer, std::string fileName);
       ~bulletObject();
       void setAngle(const double& degree) {_angle = degree;}
       double getAngle() const{return _angle;}
       void setInitialPosition(const double &x, const double& y) {_init_x_in_level = _cur_x_in_level = x, _init_y_in_level = _cur_y_in_level = y;}
       void setVelocity(const double &vx, const double& vy) {_velocity_x = vx, _velocity_y = vy;}
       bool checkMoving() const;
       void move();
       void draw(SDL_Renderer* renderer, gameMap* Map);
       double getDistance() const;
       SDL_Rect getBox() const {return _hit_box;}
       void updateBox() {_hit_box.x = _cur_x_in_level, _hit_box.y = _cur_y_in_level;}
       void changeDamage(int delta) {_damage += delta;}
       int getDamage() const {return _damage;}
private:
       double _angle;
       double _velocity_x, _velocity_y;
       double _init_x_in_level, _init_y_in_level;
       double _cur_x_in_level, _cur_y_in_level;
       int _damage;
       SDL_Rect _hit_box;
};

#endif // BULLETOBJECT_H_INCLUDED
