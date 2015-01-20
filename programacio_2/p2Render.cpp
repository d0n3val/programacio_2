#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Render.h"

p2Render::p2Render() : p2Module()
{
	strncpy(name, "render", SHORT_STR);
}

// Destructor
p2Render::~p2Render()
{
}

// Called before render is available
bool p2Render::Awake()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(App->config.GetBool("window", "vsync", false) == true)
			flags |= SDL_RENDERER_PRESENTVSYNC;

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);
	if( renderer == NULL )
	{
		LOG( "Could not create the renderer! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool p2Render::Start()
{
	LOG("render start");

	// back background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	return true;
}

// Called each loop iteration
bool p2Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool p2Render::Update(float dt)
{
	return true;
}


bool p2Render::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool p2Render::CleanUp()
{
	LOG("Destroying SDL render");
	
	SDL_DestroyRenderer(renderer);
	
	return true;
}
