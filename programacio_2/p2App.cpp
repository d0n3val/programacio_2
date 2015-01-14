#include "p2App.h"

// Constructor
p2App::p2App()
{
}

// Destructor
p2App::~p2App()
{
}

// Called before render is available
bool p2App::Awake()
{
	return true;
}

// Called before the first frame
bool p2App::Start()
{
	return true;
}

// Called each loop iteration
bool p2App::Update()
{
	return false;
}

// Called before quitting
bool p2App::CleanUp()
{
	return true;
}