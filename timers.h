#ifndef TIMERS_H_INCLUDED
#define TIMERS_H_INCLUDED
#include "utils.h"

class LTimer{
public:
       LTimer(){;}
       ~LTimer(){;}
       void start();
       void restart();
       int getTicks();
private:
       int _start_ticks;
       bool _started;
};

#endif // TIMERS_H_INCLUDED
