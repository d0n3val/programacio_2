#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

j1Window::j1Window() : j1Module(), final_title(MID_STR)
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
		int width = conf->GetInt("width", SCREEN_WIDTH);
		int height = conf->GetInt("height", SCREEN_HEIGHT);
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

// Add last frame fps stadistics in the title
bool j1Window::PreUpdate()
{
	if(App->last_fps >= 0)
	{
		final_title.create("%s - %d fps (%d/%d ms)", title.c_str(), App->last_fps, App->last_frame_ms, App->capped_ms);
		SDL_SetWindowTitle(window, final_title.c_str());
	}
	else
	{
		SDL_SetWindowTitle(window, title);
	}

	return true;
}

// Called each loop iteration
bool j1Window::Update(float dt)
{
	bool ret = true;

	if(App->input->GetWindowEvent(WE_QUIT) == true)
	{
		ret = false;
	}

	if(App->input->GetKeyDown(SDLK_ESCAPE) == true)
	{
		ret = false;
	}

	if(App->input->GetWindowEvent(WE_HIDE) == true)
	{
		if(App->pause.Get() == false)
		{
			LOG("Entering background mode");
		}

		App->pause.Set();
	}

	if(App->input->GetWindowEvent(WE_SHOW) == true)
	{
		App->pause.Unset();

		if(App->pause.Get() == false)
		{
			LOG("Leaving background mode");
		}
	}

	return ret;
}

// Swap buffers
bool j1Window::PostUpdate()
{
	return true;
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
	title.create(new_title);
}