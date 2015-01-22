#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Render.h"

p2Render::p2Render() : p2Module()
{
	strncpy_s(name, "render", SHORT_STR);

	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
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
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return ret;
}

// Called before the first frame
bool p2Render::Start()
{
	LOG("render start");

	// back background
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);

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
	int speed = 67;

	if(App->input->GetMouseButtonRepeat(SDL_BUTTON_LEFT) == true)
	{
		int x, y;
		App->input->GetMouseMotion(x,y);
		camera.x -= speed * x * dt;
		camera.y -= speed * y * dt;
	}

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

// Blit to screen
bool p2Render::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section)
{
	bool ret = true;

	SDL_Rect rect;

	rect.x = x - camera.x;
	rect.y = y - camera.y;
	
	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if(SDL_RenderCopy(renderer, texture, section, &rect) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

void p2Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}