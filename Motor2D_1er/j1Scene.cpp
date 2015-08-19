#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Render.h"
#include "j1Pathfinding.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Serialization.h"
#include "j1Window.h"
#include "j1Scene.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(j1IniReader* conf)
{
	LOG("Loading Scene");
	bool ret = true;
	
	map.create(conf->GetString("map", ""));

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	if(App->map->Load(map) == true)
	{
		int w, h;
		uchar* data = NULL;
		if(App->map->CreateWalkabilityMap(w, h, &data))
			App->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	debug_tex = App->tex->Load("maps/path.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// ---------------------------------
	if(App->last_fps >= 0)
	{
		int x, y;
		App->input->GetMousePosition(x, y);

		iPoint rel = App->render->ScreenToWorld(x, y);
		iPoint p = App->map->WorldToMap(rel.x, rel.y);

		static p2String title;

		title.create("Dev Playground - %d fps (%d/%d ms) mouse:%d,%d cam:%d,%d rel:%d,%d cell:%d,%d",
					 App->last_fps,
					 App->last_frame_ms,
					 App->capped_ms,
					 x, y,
					 App->render->camera.x, App->render->camera.y,
					 rel.x, rel.y,
					 p.x, p.y
					 );
		App->win->SetTitle(title);
	}

	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if(origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	// Draw map
	App->map->Draw();

	// Debug pathfinding
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for(uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

	// Test load/save
	if(App->input->GetKeyDown(SDLK_s))
	{
		App->serial->SaveGameState("savegame.xml");
	}

	if(App->input->GetKeyDown(SDLK_l))
	{
		App->serial->LoadGameState("savegame.xml");
	}

	// Move camera while dragging mouse
	float speed = 67.0f;

	if(App->input->GetMouseButtonRepeat(SDL_BUTTON_LEFT) == true)
	{
		int x, y;
		App->input->GetMouseMotion(x, y);
		App->render->camera.x -= (int)(speed * float(x) * dt);
		App->render->camera.y -= (int)(speed * float(y) * dt);
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKeyDown(SDLK_ESCAPE) == true)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
