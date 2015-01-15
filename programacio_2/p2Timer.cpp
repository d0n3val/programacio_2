
#include "p2Timer.h"

p2Timer::p2Timer()
{
	Start();
}

// ---------------------------------------------
void p2Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void p2Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 p2Timer::Read()
{
	if(running == true)
		return SDL_GetTicks() - started_at;
	else
		return stopped_at - started_at;
}
