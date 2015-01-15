#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

p2Window::p2Window() : p2Module()
{
	window = NULL;
	screenSurface = NULL;
	UpdateOnPause = true;
}

// Destructor
p2Window::~p2Window()
{
}

// Called before render is available
bool p2Window::Awake()
{
	LOG("Init SDL window & surface");
	bool ret = true;
	

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		LOG( "SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}
	else
	{
		//Create window
		int width = App->config.GetInt("window", "width", SCREEN_WIDTH);
		int height = App->config.GetInt("window", "height", SCREEN_HEIGHT);

		window = SDL_CreateWindow( "P2 Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN );
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
	bool ret = true;

	if(App->input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(App->input->GetWindowEvent(WE_HIDE) == true)
	{
		if(App->pause.Get() == false)
			LOG("Entering background mode");

		App->pause.Set();
	}

	if(App->input->GetWindowEvent(WE_SHOW) == true)
	{
		App->pause.Unset();

		if(App->pause.Get() == false)
			LOG("Leaving background mode");
	}

	return ret;
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