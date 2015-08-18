#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Load Game State
bool j1Render::LoadGameState(j1Serialization* serial)
{
	camera.x = serial->LoadInt("cam_x");
	camera.y = serial->LoadInt("cam_y");

	return true;
}

// Save Game State
bool j1Render::SaveGameState(j1Serialization* serial)
{
	serial->SaveInt("cam_x", camera.x);
	serial->SaveInt("cam_y", camera.y);

	return true;
}

// Called before render is available
bool j1Render::Awake(j1IniReader* conf)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(conf->GetBool("vsync", false) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
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
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	if(App->input->GetKeyDown(SDLK_s))
	{
		App->serial->SaveGameState("savegame.xml");
	}

	if(App->input->GetKeyDown(SDLK_l))
	{
		App->serial->LoadGameState("savegame.xml");
	}

	float speed = 67.0f;

	if(App->input->GetMouseButtonRepeat(SDL_BUTTON_LEFT) == true)
	{
		int x, y;
		App->input->GetMouseMotion(x, y);
		camera.x -= (int) (speed * float(x) * dt);
		camera.y -= (int) (speed * float(y) * dt);
	}

	return true;
}

bool j1Render::PostUpdate()
{
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section)
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

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}


p2Point<int> j1Render::ScreenToWorld(int x, int y) const
{
	p2Point<int> ret;

	ret.x = x + camera.x;
	ret.y = y + camera.y;

	return ret;
}