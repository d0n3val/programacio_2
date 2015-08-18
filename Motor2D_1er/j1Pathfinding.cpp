#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : 
j1Module(), 
map(NULL), 
last_path(DEFAULT_PATH_LENGTH),
width(0), 
height(0),
debug(true)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before render is available
bool j1PathFinding::Awake(j1IniReader* conf)
{
	LOG("Init Pathfinding library");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1PathFinding::Start()
{
	LOG("Start pathfinding");
	bool ret = true;

	debug_tex = App->tex->Load("maps/path.png");

	return ret;
}

// Called each loop iteration
bool j1PathFinding::PreUpdate()
{
	if(debug == true)
	{
		static p2Point<int> origin;
		static bool origin_selected = false;

		int x, y;
		App->input->GetMousePosition(x, y);
		p2Point<int> p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);

		if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
		{
			if(origin_selected == true)
			{		
				CreatePath(origin, p);
				origin_selected = false;
			}
			else
			{
				origin = p;
				origin_selected = true;
			}
		}
	}
	
	return true;
}

bool j1PathFinding::Update(float dt)
{
	if(debug == true)
	{
		int x, y;
		App->input->GetMousePosition(x, y);
		p2Point<int> p = App->render->ScreenToWorld(x, y);
		p = App->map->WorldToMap(p.x, p.y);
		p = App->map->MapToWorld(p.x, p.y);

		App->render->Blit(debug_tex, p.x, p.y);

		for(uint i = 0; i < last_path.Count(); ++i)
		{
			p2Point<int> pos = App->map->MapToWorld(last_path[i].x, last_path[i].y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}

	return true;
}


bool j1PathFinding::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");
	return true;
}

// Set the map to do the pathfinding
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

int j1PathFinding::CreatePath(const p2Point<int>& origin, const p2Point<int>& destination)
{
	int ret = -1;

	if(CheckBoundaries(origin) && CheckBoundaries(destination))
	{
		last_path.Clear();

		last_path.PushBack(origin);
		last_path.PushBack(destination);
		ret = 2;
	}

	return ret;
}

bool j1PathFinding::CheckBoundaries(const p2Point<int>& pos) const
{
	return (pos.x >= 0 && pos.x <= width &&
			pos.y >= 0 && pos.y <= height);
}

const p2DynArray<p2Point<int>> j1PathFinding::GetLastPath() const
{
	return last_path;
}