#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "zlib/include/zlib.h"
#include "zlib/include/zconf.h"

#pragma comment( lib, "zlib/libx86/zdll.lib" )

j1FileSystem::j1FileSystem() : j1Module()
{
	name.create("file_system");
}

// Destructor
j1FileSystem::~j1FileSystem()
{}

// Called before render is available
bool j1FileSystem::Awake()
{
	LOG("Loading File System");
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1FileSystem::CleanUp()
{
	LOG("Freeing File System subsystem");

	return true;
}
