#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(SDL_Renderer* renderer, std::string fileName){
       SDL_Surface* tempSurface = IMG_Load(fileName.c_str());

       if(tempSurface == NULL){
              CommonFunction::logSDLError(std::cout, "Load Image from " + fileName, true);
       }

       SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

       SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
       if(texture == NULL){
              CommonFunction::logSDLError(std::cout, "Create Texture from " + fileName, true);
       }

       SDL_FreeSurface(tempSurface);

       return texture;
}

SDL_Texture* TextureManager::LoadText(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color){
       SDL_Surface* tempSurface = TTF_RenderText_Solid(font, text.c_str(), color);

       if(tempSurface == NULL){
              CommonFunction::logSDLError(std::cout, "Load text: " + text, true);
       }

       SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
       if(texture == NULL) CommonFunction::logSDLError(std::cout, "Create text texure", true);

       SDL_FreeSurface(tempSurface);

       return texture;
}

