#include "gate.h"

void Gate::draw(SDL_Renderer* renderer, gameMap* Map){
       setRect(getBox().x - Map->getCurrentCameraX() - 32, getBox().y - Map->getCurrentCameraY() - 32);
       render(renderer);
}
