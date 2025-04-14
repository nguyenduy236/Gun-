#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

#include "utils.h"
#include "base_object.h"

class Cursor : public baseObject{
public:
       Cursor(SDL_Renderer* renderer);
       ~Cursor(){;}
       void update();
       void setCurrentClip(const int &c) {_current_clip = c;}
       void draw(SDL_Renderer* renderer);
       int getMouseX() const {return _pos_x;}
       int getMouseY() const {return _pos_y;}
private:
       int _pos_x, _pos_y;
       SDL_Rect _sprite[2];
       int _current_clip;
};

#endif // CURSOR_H_INCLUDED

