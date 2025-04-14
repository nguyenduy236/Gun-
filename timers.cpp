#include "timers.h"

void LTimer::start(){
       _started = true;
       _start_ticks = SDL_GetTicks();
}

void LTimer::restart(){
       _start_ticks = SDL_GetTicks();
}

int LTimer::getTicks(){
       if(!_started) return 0;
       return SDL_GetTicks() - _start_ticks;
}
