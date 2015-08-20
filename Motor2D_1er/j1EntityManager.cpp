// ----------------------------------------------------
// Controls all logic entities (enemies/player/etc.) --
// ----------------------------------------------------

#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Entity.h"
#include "j1EntityManager.h"

j1EntityManager::j1EntityManager() : j1Module(), logic_updates_per_second(DEFAULT_LOGIC_PER_SECOND), accumulated_time(0.0f), entities(dummy)
{
	name.create("entities");
	dummy = new j1Entity();
	entities.trunk = dummy;
}

// Destructor
j1EntityManager::~j1EntityManager()
{}

// Called before render is available
bool j1EntityManager::Awake(j1IniReader* conf)
{
	LOG("Setting up Entity manager");
	bool ret = true;
	logic_updates_per_second = conf->GetInt("updates_per_second ", DEFAULT_LOGIC_PER_SECOND);
	update_ms_cycle = 1.0f / (float)logic_updates_per_second;
	return ret;
}

// Called before the first frame
bool j1EntityManager::Start(j1IniReader* conf)
{
	LOG("start j1EntityManager");
	bool ret = true;
	return ret;
}

// Called each loop iteration
bool j1EntityManager::PreUpdate()
{
	do_logic = false;
	return true;
}

bool j1EntityManager::Update(float dt)
{
	accumulated_time += dt;

	if(accumulated_time >= update_ms_cycle)
	{
		do_logic = true;
	}

	UpdateRecursive(dt, &entities.trunk);

	if(do_logic == true)
	{
		//LOG("Did logic step after %f", accumulated_time);
		accumulated_time = 0.0f;
	}

	return true;
}

void j1EntityManager::UpdateRecursive(float dt, p2TreeNode<j1Entity*>* node)
{
	j1Entity* entity = node->data;

	if(entity->IsActive() == true)
	{
		entity->FixedUpdate(dt);

		if(do_logic == true)
		{
			entity->LogicUpdate(accumulated_time);
		}

		p2List_item<p2TreeNode<j1Entity*>*>* item = node->childs.start;
		for(; item != NULL; item = item->next)
		{
			UpdateRecursive(dt, item->data);
		}
	}
}

bool j1EntityManager::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("cleanup j1EntityManager");
	// release all entities
	p2List<j1Entity*> list;
	entities.trunk.GatherAllData(&list);
	p2List_item<j1Entity*>* item = list.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	entities.Clear();
	return true;
}

// Create a new empty entity
j1Entity* const j1EntityManager::Add()
{
	j1Entity* entity = new j1Entity;
	entity->Init(this);
	entities.Add(entity);
	return(entity);
}

