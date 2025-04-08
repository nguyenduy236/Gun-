#include "text.h"

Text::Text(std::string name, int size) : _file_name(name), _current_size(size){
       _gFont = TTF_OpenFont( ("data/font/" + _file_name).c_str(), _current_size );
       _text_color = {255, 255, 255};
}

Text::~Text(){
       if(_gFont != NULL){
              TTF_CloseFont(_gFont);
              _gFont = NULL;
       }
}

void Text::resize(int size){

       if(_gFont != NULL){
              delete _gFont;
              _gFont = NULL;
       }
       if(_current_size == size) return;
       _current_size = size;
       _gFont = TTF_OpenFont( ("data/font/" + _file_name).c_str(), _current_size );

}

void Text::loadText(SDL_Renderer* renderer, std::string text){

       free();
       _pObject = TextureManager::LoadText(renderer, _gFont, text, _text_color);
       SDL_QueryTexture(_pObject, NULL, NULL, &_rect.w, &_rect.h);

}

void Text::draw(SDL_Renderer* renderer, const int& x, const int& y){
       setRect(x, y);
       render(renderer);
}
