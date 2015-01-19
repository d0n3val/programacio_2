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
			case SDL_KEYUP:

				int code = event.key.keysym.sym;
				p2KeyState state = KS_IDLE;

				if(event.key.repeat != 0)
					state = KS_REPEAT;
				else
					if(event.key.state == SDL_PRESSED)
						state = KS_DOWN;
					else
						state = KS_UP;

				if(	code > 127) 
				{
					code -= (127 + 1073741881); // https://wiki.libsdl.org/SDLKeycodeLookup
				}

				keyState[code] = state;
				LOG("Key %d changes state to %d", code, state);
					
			break;
		 }
    }

	return true;
}

// Called before quitting
bool p2Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

// ---------
void p2Input::CleanKeys()
{
	for(int i = 0; i < WE_COUNT; ++i)
		windowEvents[i] = false;

	for(int i = 0; i < NUM_KEYS; ++i)
		keyState[i] = KS_IDLE;
}

bool p2Input::GetWindowEvent(p2EventWindow ev)
{
	return windowEvents[ev];
}

bool p2Input::GetKeyDown(int code)
{
	return keyState[code] == KS_DOWN;
}

bool p2Input::GetKeyRepeat(int code)
{
	return keyState[code] == KS_REPEAT;
}

bool p2Input::GetKeyUp(int code)
{
	return keyState[code] == KS_UP;
}