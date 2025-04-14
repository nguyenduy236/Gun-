#include "button.h"

Button::Button(){
       _clicked = false;
       _active = false;
}

Button::~Button(){

}

void Button::handleInputAction(const SDL_Event& e, Cursor* mouse){
       if(!isActivated()){

           this->setColor(255, 255, 255);
           return;

       }

       int x = mouse->getMouseX(), y = mouse->getMouseY();

       if(e.type == SDL_MOUSEMOTION){

              if(CommonFunction::checkPointInsideRect(x, y, this->getRect())) this->setColor(128, 128, 128);
              else this->setColor(255, 255, 255);

       }
       else if(e.type == SDL_MOUSEBUTTONDOWN){

              if(CommonFunction::checkPointInsideRect(x, y, this->getRect())) setClicked(true);

       }
}

