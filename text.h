#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "utils.h"
#include "base_object.h"

class Text : public baseObject{
public:
       Text(){_gFont = NULL;}
       Text(std::string name, int size);
       ~Text();
       void setTextColor(const SDL_Color& color) {_text_color = color;}
       void resize(int size);
       void loadText(SDL_Renderer* renderer, const std::string text);
       void draw(SDL_Renderer* renderer, const int &x, const int& y);
private:
       int _current_size;
       SDL_Color _text_color;
       std::string _file_name;
       TTF_Font* _gFont;
};

#endif // TEXT_H_INCLUDED
