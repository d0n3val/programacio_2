#include "p2Defs.h"
#include "p2Log.h"
#include "p2Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

p2Window::p2Window()
{
	window = NULL;
	screenSurface = NULL;
}

// Destructor
p2Window::~p2Window()
{
}

// Called before render is available
bool p2Window::Awake()
{
	LOG("window awake: creating window with SDL");
	bool ret = true;
	

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		LOG( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "P2 Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			LOG( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			ret = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );
		}
	}

	return ret;
}

// Called before the first frame
bool p2Window::Start()
{
	LOG("window start");
	return true;
}

// Called each loop iteration
bool p2Window::Update(float dt)
{
	LOG("window update");
	return false;
}

// Called before quitting
bool p2Window::CleanUp()
{
	LOG("window cleanup");
	
	//Destroy window
	if(window != NULL)
		SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return true;
}