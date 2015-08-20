#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

#include "SDL/include/SDL.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	update_on_pause = true;
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{
	name.destroy();
}

// Called before render is available
bool j1Window::Awake(j1IniReader* conf)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = conf->GetInt("width", SCREEN_WIDTH);
		height = conf->GetInt("height", SCREEN_HEIGHT);
		scale = conf->GetInt("scale", 1);

		width *= scale;
		height *= scale;

		title = conf->GetString("title", "No title");
		Uint32 flags = SDL_WINDOW_SHOWN;

		if(conf->GetBool("fullscreen", false) == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(conf->GetBool("borderless", false) == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(conf->GetBool("resizable", false) == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(conf->GetBool("fullscreen_windowed", false) == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint j1Window::GetScale() const
{
	return scale;
}