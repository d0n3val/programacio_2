
#include "j1Timer.h"

j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void j1Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 j1Timer::Read()
{
	if(running == true) {
		return SDL_GetTicks() - started_at;
	}
	else {
		return stopped_at - started_at;
	}
}
