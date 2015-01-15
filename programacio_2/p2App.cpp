#include "p2Log.h"
#include "p2App.h"
#include "p2Window.h"
#include "p2Input.h"


// Constructor
p2App::p2App(const char* config_file)
{
	quitting = false;
	config.SetFile(config_file);

	input = new p2Input();
	win = new p2Window();
	
	// Put in order of awake / Start / Update
	// Reverse order of CleanUp

	AddModule(input);
	AddModule(win);
}

// Destructor
p2App::~p2App()
{
	modules.clear();
}

void p2App::AddModule(p2Module* module)
{
	module->Init(this);
	modules.add(module);
}

// Called before render is available
bool p2App::Awake()
{
	bool ret = true;

	p2list_item<p2Module*>* item;

	item = modules.start;
	while( item != NULL && ret == true )
	{
		ret = item->data->Awake();
		item = item->next;
	}

	return ret;
}

// Called before the first frame
bool p2App::Start()
{
	bool ret = true;

	p2list_item<p2Module*>* item;

	item = modules.start;
	while( item != NULL && ret == true )
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool p2App::Update()
{
	bool ret = true;
	float dt = 0.0f;

	p2list_item<p2Module*>* item;

	item = modules.start;
	p2Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->Active == false)
			continue;

		if(pause.Get() == true && pModule->UpdateOnPause == false)
			continue;

		ret = item->data->Update(dt);
	}

	return ret;
}

// Called before quitting
bool p2App::CleanUp()
{
	bool ret = true;

	p2list_item<p2Module*>* item;

	item = modules.end;
	while( item != NULL && ret == true )
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

