#include "p2Defs.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "j1EntityManager.h"
#include "j1FileSystem.h"
#include "j1Serialization.h"
#include "j1PathFinding.h"
#include "j1Gui.h"

#include "j1App.h"

// Constructor
j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	quitting = false;
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = -1;
	fps_counter = 0;

	fs = new j1FileSystem(argc > 0 ? args[1] : "./");
	input = new j1Input();
	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	fonts = new j1Fonts();
	audio = new j1Audio();
	map = new j1Map();
	entities = new j1EntityManager();
	serial = new j1Serialization();
	pathfinding = new j1PathFinding();
	gui = new j1Gui();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(fs);
	AddModule(serial);
	AddModule(input);
	AddModule(win);
	AddModule(render);
	AddModule(tex);
	AddModule(fonts);
	AddModule(audio);
	AddModule(map);
	AddModule(entities);
	AddModule(pathfinding);
	AddModule(gui);
}

// Destructor
j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	ASSERT(module);
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = true;

	char* buffer;
	unsigned int s = fs->Load("config.ini", &buffer);
	config.SetBuffer(buffer, s);
	RELEASE(buffer);

	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if(config.SetSection(item->data->name.c_str()) == true)
			ret = item->data->Awake(&config);
		else
			ret = item->data->Awake(NULL);

		item = item->next;
	}

	// We configure App the last to allow fs to self.configure
	config.SetSection("App");

	int cap = config.GetInt("framerate_cap", -1);

	if(cap > 0)
	{
		capped_ms = 1000 / cap;
	}

	organization = config.GetString("organization", "MyOrganization");
	app_name = config.GetString("app_name", "MyAppName");

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	bool ret = true;
	PrepareUpdate();
	ret = PreUpdate();

	if(ret == true) {
		ret = DoUpdate();
	}

	if(ret == true) {
		ret = PostUpdate();
	}

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
void j1App::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void j1App::FinishUpdate()
{
	// In background mode leave some time for OS to do stuff
	if(pause.Get() == true) {
		SDL_Delay(33);
	}

	// Recap on framecount and fps
	++frames;
	++fps_counter;

	if(fps_timer.Read() >= 1000)
	{
		last_fps = fps_counter;
		fps_counter = 0;
		fps_timer.Start();
	}

	last_frame_ms = ms_timer.Read();

	// cap fps
	if(last_frame_ms < capped_ms) {
		SDL_Delay(capped_ms - last_frame_ms);
	}
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if(pause.Get() == true && pModule->update_on_pause == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if(pause.Get() == true && pModule->update_on_pause == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		if(pause.Get() == true && pModule->update_on_pause == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Call all modules to load a game state
bool j1App::LoadGameState()
{
	bool ret = true;
	p2List_item<j1Module*>* item = modules.start;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		if(serial->SetLoadSection(item->data->name))
			ret = item->data->LoadGameState(serial);
	}

	return ret;
}

// Call all modules to save their game state
bool j1App::SaveGameState()
{
	bool ret = true;
	p2List_item<j1Module*>* item = modules.start;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		serial->SetWriteSection(item->data->name);
		ret = item->data->SaveGameState(serial);
	}

	return ret;
}


// Called before quitting
bool j1App::CleanUp()
{
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
const char* j1App::GetOrganizationName() const
{
	return organization;
}

// ---------------------------------------
const char* j1App::GetAppName() const
{
	return app_name;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}