#include "p2Defs.h"
#include "p2Log.h"
#include "p2Input.h"

p2Input::p2Input()
{
}

// Destructor
p2Input::~p2Input()
{
}

// Called before render is available
bool p2Input::Awake()
{
	LOG("Init SDL event system");
	bool ret = true;

	SDL_Init(0);
	if( SDL_InitSubSystem( SDL_INIT_EVENTS ) < 0 )
	{
		LOG( "SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}

	// SDL_INIT_JOYSTICK ?
	// SDL_INIT_GAMECONTROLLER ?

	return ret;
}

// Called before the first frame
bool p2Input::Start()
{
	LOG("input start");
	return true;
}

// Called each loop iteration
bool p2Input::Update(float dt)
{
	CleanKeys();
	SDL_Event event;

    while (SDL_PollEvent(&event) != 0) 
	{
         switch(event.type)
		 {
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
						windowEvents[WE_HIDDEN] = true;
						LOG("minimized");
				}
			
				
			break;
		 }
    }

	return true;
}

// Called before quitting
bool p2Input::CleanUp()
{
	LOG("input cleanup");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

// ---------
void p2Input::CleanKeys()
{
	for(int i = 0; i < WE_COUNT; ++i)
		windowEvents[i] = false;

}

bool p2Input::GetWindowEvent(p2EventWindow ev)
{
	return windowEvents[ev];
}
