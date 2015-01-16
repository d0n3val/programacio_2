#include "p2Defs.h"
#include "p2Log.h"
#include "p2Input.h"

p2Input::p2Input() : p2Module()
{
	UpdateOnPause = true;
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
bool p2Input::PreUpdate()
{
	static SDL_Event event;
	CleanKeys();

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
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
						windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_KEYDOWN:
				if(event.key.keysym.sym == SDLK_ESCAPE)
					windowEvents[WE_QUIT] = true;
			break;
		 }
    }

	return true;
}

bool p2Input::Update(float dt)
{
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
