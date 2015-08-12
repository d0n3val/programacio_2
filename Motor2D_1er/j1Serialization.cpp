#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Serialization.h"

j1Serialization::j1Serialization() : j1Module()
{
	name.create("serialization");
}

// Destructor
j1Serialization::~j1Serialization()
{
}

// Called before render is available
bool j1Serialization::Awake(j1IniReader* conf)
{
	LOG("Loading Serialization System");
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Serialization::CleanUp()
{
	LOG("Freeing Serialization subsystem");

	return true;
}
