#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "p2RefBool.h"
#include "j1Timer.h"
#include "p2String.h"
#include "j1IniReader.h"
#include "j1Module.h"

// Modules
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

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Load Game State
	bool LoadGameState();

	// Save Game State
	bool SaveGameState();

	// Add a new module to handle
	void AddModule(j1Module* module);

	const char* GetOrganizationName() const;
	const char* GetAppName() const;
	int GetArgc() const;
	const char* GetArgv(int index) const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	p2RefBool	pause;
	j1IniReader	config;
	Uint32		frames;
	float		dt;
	int			fps_counter;
	int			last_frame_ms;
	int			last_fps;
	int			capped_ms;

	// Modules
	j1Window*			win;
	j1Input*			input;
	j1Render*			render;
	j1Textures*			tex;
	j1Fonts*			fonts;
	j1Audio*			audio;
	j1Map*				map;
	j1EntityManager*	entities;
	j1FileSystem*		fs;
	j1Serialization*	serial;
	j1PathFinding*		pathfinding;

private:

	bool				quitting;
	j1Timer				ms_timer;
	j1Timer				fps_timer;
	p2List<j1Module*>	modules;
	p2String			organization;
	p2String			app_name;
	int					argc;
	char**				args;
};

#endif