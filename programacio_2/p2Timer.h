#ifndef __P2TIMER_H__
    #define __P2TIMER_H__

#include "p2Defs.h"
#include "SDL\include\SDL.h"

class p2Timer
{
public:

    // Constructor
    p2Timer();

    void Start();
    void Stop();

	Uint32 Read();

private:

    bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
};

#endif //__P2TIMER_H__