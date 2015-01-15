#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

p2Window::p2Window() : p2Module()
{
	window = NULL;
	screen_surface = NULL;
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
		char* title = App->config.GetString("window", "title", "P2 Engine");

		Uint32 flags = SDL_WINDOW_SHOWN;

		if(App->config.GetBool("window", "fullscreen", false) == true)
			flags |= SDL_WINDOW_FULLSCREEN;

		if(App->config.GetBool("window", "borderless", false) == true)
			flags |= SDL_WINDOW_BORDERLESS;

		if(App->config.GetBool("window", "resizable", false) == true)
			flags |= SDL_WINDOW_RESIZABLE;

		if(App->config.GetBool("window", "fullscreen_windowed", false) == true)
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags );
		if( window == NULL )
		{
			LOG( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface( window );

			SetTitle(title);
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

	SDL_UpdateWindowSurface( window );

	char framerate[255];
	sprintf_s(framerate, "%s - %d fps (%d/%d ms | %.3f dt)", title, App->last_fps, App->last_frame_ms, App->capped_ms, dt);// SDL_GetWindowTitle(window));//, App->last_frametime);
	SDL_SetWindowTitle(window, framerate);

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

// Set new window title
void p2Window::SetTitle(const char* new_title)
{
	strcpy_s(title, 255, new_title);
}