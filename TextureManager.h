#ifndef TEXTUREMANAGER_H_INCLUDED
#define TEXTUREMANAGER_H_INCLUDED

#include "utils.h"

class TextureManager{
public:
       static SDL_Texture* LoadTexture(SDL_Renderer* renderer, std::string fileName);
       static SDL_Texture* LoadText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color);
};

#endif // TEXTUREMANAGER_H_INCLUDED
