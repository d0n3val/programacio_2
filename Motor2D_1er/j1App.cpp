#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"

// Constructor
j1App::j1App(const char* config_file)
{
	quitting = false;
	frames = 0;
	last_frame_ms = -1;
	last_fps = -1;
	capped_ms = -1;
	fps_counter = 0;

	config.SetFile(config_file);

	input = new j1Input();

	win = new j1Window();
	render = new j1Render();
	tex = new j1Textures();
	fonts = new j1Fonts();
	audio = new j1Audio();
	map = new j1Map();
	entities = new j1EntityManager();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(win);
	AddModule(render);
	AddModule(tex);
	AddModule(fonts);
	AddModule(audio);
	AddModule(map);
	AddModule(entities);
}

// Destructor
j1App::~j1App()
{}

void j1App::AddModule(j1Module* module)
{
	ASSERT(module);

	module->Init(this);
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	bool ret = true;

	int cap = config.GetInt("App", "framerate_cap", -1);
	if(cap > 0)
	{
		capped_ms = 1000 / cap;
	}

	p2List_item<j1Module*>* item;

	item = modules.start;
	while(item != NULL && ret == true)
	{
		ret = item->data->Awake();
		item = item->next;
	}

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

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();

	return ret;
}

void j1App::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

void j1App::FinishUpdate()
{
	// In background mode leave some time for OS to do stuff
	if(pause.Get() == true)
		SDL_Delay(33);

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
	if(last_frame_ms < capped_ms)
		SDL_Delay(capped_ms - last_frame_ms);
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

		if(pModule->active == false)
			continue;

		if(pause.Get() == true && pModule->update_on_pause == false)
			continue;

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

		if(pModule->active == false)
			continue;

		if(pause.Get() == true && pModule->update_on_pause == false)
			continue;

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	bool ret = true;

	p2List_item<j1Module*>* item;

	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false)
			continue;

		if(pause.Get() == true && pModule->update_on_pause == false)
			continue;

		ret = item->data->PostUpdate();
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

	// release modules
	item = modules.end;
	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}
	modules.clear();

	return ret;
}

