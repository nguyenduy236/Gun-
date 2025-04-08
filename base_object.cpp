#include "base_object.h"

baseObject::baseObject(){
       _pObject = NULL;
}

baseObject::baseObject(SDL_Renderer* renderer, std::string fileName){
       _pObject = TextureManager::LoadTexture(renderer, "data/image/" + fileName);
       if(_pObject == NULL) CommonFunction::logSDLError(std::cout, "Load " + fileName, true);
       SDL_QueryTexture(_pObject, NULL, NULL, &_rect.w, &_rect.h);
       _rect.x = _rect.y = 0;
}

baseObject::~baseObject(){
       free();
}

void baseObject::setRect(const int x, const int y){
       _rect.x = x;
       _rect.y = y;
}

void baseObject::setAlpha(const int& a){ // điều chỉnh độ trong suốt
       SDL_SetTextureAlphaMod(_pObject, a);
}

void baseObject::loadFromFile(SDL_Renderer* renderer, std::string fileName){
       free();
       _pObject = TextureManager::LoadTexture(renderer, "data/image/" + fileName);
       if(_pObject == NULL) CommonFunction::logSDLError(std::cout, "Load " + fileName, true);
       SDL_QueryTexture(_pObject, NULL, NULL, &_rect.w, &_rect.h);
}

void baseObject::render(SDL_Renderer *renderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){

        SDL_Rect renderQuad = _rect;
        if(clip != NULL) renderQuad.w = clip->w, renderQuad.h = clip->h;

        SDL_RenderCopyEx(renderer, _pObject, clip, &renderQuad, angle, center, flip);

}

void baseObject::free(){

       if(_pObject != NULL){

              SDL_DestroyTexture(_pObject);
              _pObject = NULL;
              _rect = {0, 0, 0, 0};

       }

}
