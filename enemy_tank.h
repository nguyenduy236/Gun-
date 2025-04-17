#ifndef ENEMYTANK_H_INCLUDED
#define ENEMYTANK_H_INCLUDED

#include "utils.h"
#include "bulletObject.h"
#include "gun_tank.h"
#include "timers.h"

class enemyTank : public baseObject{
public:
       enemyTank(int iAttackRange, int iHealthPoint, int iX, int iY, SDL_Renderer* renderer);
       ~enemyTank();
       int getX() const {return _x_in_level;}
       int getY() const {return _y_in_level;}
       void setAttackRange(const int& ar) {_attack_range = ar;}
       int getAttackRange() const {return _attack_range;}
       void draw(SDL_Renderer *renderer, gameMap *camera, const int &main_tank_x, const int &main_tank_y);
       void move();
       void move_back();
       bool checkValidPos(gameMap *Map) const;
       void handleShot(SDL_Renderer* renderer, const int &pos_x, const int &pos_y);
       void findPath(int** shortest_path); // trace main Tank
       void handleBullet(SDL_Renderer* renderer, gameMap* Map);
       void updateBox() {_hit_box.x = _x_in_level + 4, _hit_box.y = _y_in_level + 4;}
       void flicker();
       void resetFlicker() {_i_flicker = 0;}
       SDL_Rect getBox() const {return _hit_box;}
       void changeHealth(const int &delta) {_health_point += delta;}
       int getHealth() const {return _health_point;}
       bool isLiving() const {return _health_point > 0;}
       std::vector<bulletObject*> getBulletList() const {return _bullet_list;}
       void setBulletList(const std::vector<bulletObject*> &new_bullet_list) {_bullet_list = new_bullet_list;}
       void renderHPBar(SDL_Renderer* renderer, const int &x, const int& y, const int &w, const int &h, SDL_Color color);
       void getCenter(int *x, int *y){*x = _x_in_level + TILE_SIZE / 2, *y = _y_in_level + TILE_SIZE / 2;}
       void setMaxHealth(const int &maxHP) {_max_health_point = maxHP;}
       bool fitToCell();
private:

       directState _current_direction;
       int _velocity_x, _velocity_y;
       int _x_in_level, _y_in_level;
       int _attack_range;
       int _health_point, _max_health_point;
       int _i_flicker;
       LTimer* _timer_shot; // handle attack speed

       std::vector<bulletObject*> _bullet_list;
       gunTank* _gun;
       SDL_Rect _hit_box;

};

#endif // ENEMYTANK_H_INCLUDED
