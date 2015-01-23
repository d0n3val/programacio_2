#ifndef __j1TIMER_H__
    #define __j1TIMER_H__

#include "p2Defs.h"
#include "SDL\include\SDL.h"

class j1Timer
{
public:

    // Constructor
    j1Timer();

    void Start();
    void Stop();

	Uint32 Read();

private:

    bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
};

#endif //__j1TIMER_H__