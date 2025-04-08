#ifndef GATE_H_INCLUDED
#define GATE_H_INCLUDED

#include "utils.h"
#include "base_object.h"
#include "game_map.h"

class Gate : public baseObject{
public:
       Gate();
       Gate(SDL_Renderer* renderer, std::string fileName) : baseObject(renderer, fileName) {;}
       ~Gate(){;}
       void draw(SDL_Renderer* renderer, gameMap* Map);
       void setBox(const int &x, const int &y, const int &w = TILE_SIZE, const int &h = TILE_SIZE){_hit_box.x = x, _hit_box.y = y;}
       SDL_Rect getBox() const {return _hit_box;}
private:
       SDL_Rect _hit_box;
};

#endif // GATE_H_INCLUDED
