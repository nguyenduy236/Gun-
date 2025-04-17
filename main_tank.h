#ifndef MAIN_TANK_H_INCLUDED
#define MAIN_TANK_H_INCLUDED

#include "utils.h"
#include "game_map.h"
#include "base_object.h"
#include "bulletObject.h"
#include "gun_tank.h"
#include "cursor.h"

class mainTank: public baseObject{
public:

       mainTank(int iAttackRange, int iHealthPoint, int iX, int iY, SDL_Renderer* renderer);
       ~mainTank();
       int getX() const {return _x_in_level;}
       int getY() const {return _y_in_level;}
       void setAttackRange(const int& ar) {_attack_range = ar;}
       int getAttackRange() const {return _attack_range;}
       void draw(SDL_Renderer *renderer, gameMap *camera);
       void handleInputAction(const SDL_Event& e, SDL_Renderer* renderer, gameMap* Map, Cursor* mouse);
       void handleKeyState();
       void move();
       void move_back();
       void siteCamera(gameMap* Camera); // camera di chuyen theo nhan vat
       bool checkValidPos(gameMap *Map) const;
       void handleBullet(SDL_Renderer* renderer, gameMap* Map);
       void updateBox() {_hit_box.x = _x_in_level + 4, _hit_box.y = _y_in_level + 4;}
       SDL_Rect getBox() const {return _hit_box;}
       void changeHealth(const int &delta) {_health_point += delta; if(_health_point > _max_health_point) _health_point = _max_health_point;}
       bool isLiving() const {return _health_point > 0;}
       int getHealth() const {return _health_point;}
       std::vector<bulletObject*> getBulletList() const {return _bullet_list;}
       void setBulletList(const std::vector<bulletObject*> &new_bullet_list) {_bullet_list = new_bullet_list;}
       void flicker();
       void renderHPBar(SDL_Renderer* renderer, const int &x, const int& y, const int &w, const int &h, SDL_Color color);
       void resetFlicker() {_i_flicker = 0;}
       void getCenter(int *x, int *y){*x = _x_in_level + TILE_SIZE / 2, *y = _y_in_level + TILE_SIZE / 2;}
       bool fitToCell();
       int **getShortestPath() const {return _shortest_path;}
       void updateShortestPath(gameMap* Map);
private:

       directState _current_direction;
       int _velocity_x, _velocity_y;
       int _x_in_level, _y_in_level;
       int _attack_range;
       int _health_point, _max_health_point;
       int _i_flicker;

       std::vector<bulletObject*> _bullet_list;
       gunTank* _gun;
       SDL_Rect _hit_box;
       Mix_Chunk* _shot_sound;

       int** _shortest_path;
};

#endif // MAIN_TANK_H_INCLUDED
