#ifndef GUN_TANK_H_INCLUDED
#define GUN_TANK_H_INCLUDED

#include "utils.h"
#include "base_object.h"

class gunTank : public baseObject{
private:
       double _angle;
public:
       gunTank(SDL_Renderer* renderer, std::string fileName);
       ~gunTank(){;}
       void setAngle(const double &angle) {_angle = angle;}
       double getAngle() const {return _angle;}
       void draw(SDL_Renderer* renderer);
};

#endif // GUN_TANK_H_INCLUDED
