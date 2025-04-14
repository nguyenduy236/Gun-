#include "cursor.h"

Cursor::Cursor(SDL_Renderer* renderer) : baseObject(renderer, "cursor.png"){
       _sprite[0] = {0, 0, 25, 25};
       _sprite[1] = {25, 0, 25, 25};
       SDL_ShowCursor(false);
       _current_clip = 0;
}

void Cursor::update(){
       SDL_GetMouseState(&_pos_x, &_pos_y);
       setRect(_pos_x, _pos_y);
}

void Cursor::draw(SDL_Renderer* renderer){
       render(renderer, &_sprite[_current_clip]);
}

