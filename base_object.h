#ifndef BASE_OBJECT_H_INCLUDED
#define BASE_OBJECT_H_INCLUDED

#include "utils.h"
#include "TextureManager.h"

class baseObject{
public:
       baseObject();
       baseObject(SDL_Renderer* renderer, std::string fileName);
       ~baseObject();
       void setRect(const int x, const int y);
       void loadFromFile(SDL_Renderer* renderer, std::string fileName);
       void setColor(int red, int green, int blue) {SDL_SetTextureColorMod(_pObject, red, green, blue);}
       void setAlpha(const int &a);
       void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
       SDL_Texture* getTexture(){return _pObject;}
       SDL_Rect& getRect() {return _rect;}
       void free();
protected:
       SDL_Texture* _pObject;
       SDL_Rect _rect;
};

#endif // BASE_OBJECT_H_INCLUDED
