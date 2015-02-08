#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

#include "p2Defs.h"
#include "SDL\include\SDL.h"

struct j1PerfTimer
{
public:

	// Constructor
	j1PerfTimer()
	{
		frequency = (double)SDL_GetPerformanceFrequency() / 1000.0f;
		Start();
	}

	inline void Start()
	{
		stopped_at = 0;
		started_at = SDL_GetPerformanceCounter();
	}

	inline void Stop()
	{
		stopped_at = SDL_GetPerformanceCounter();
	}

	inline double ReadMs()
	{
		return (double)ReadTicks() / frequency;
	}

	inline Uint64 ReadTicks()
	{
		return (stopped_at == 0) ? SDL_GetPerformanceCounter() - started_at : stopped_at - started_at;
	}

	Uint64	started_at;
	Uint64	stopped_at;
	double	frequency;
};

#endif //__j1PERFTIMER_H__