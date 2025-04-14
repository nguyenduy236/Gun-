#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "utils.h"
#include "base_object.h"
#include "cursor.h"

class Button : public baseObject{
public:
       Button();
       ~Button();
       void handleInputAction(const SDL_Event& e, Cursor* mouse);
       void setClicked(bool click) {_clicked = click;}
       bool isClicked() const{return _clicked;}
       void activate(const bool &active) {_active = active;}
       bool isActivated() const{return _active;}
private:
       bool _clicked;
       bool _active;
};

#endif // BUTTON_H_INCLUDED
