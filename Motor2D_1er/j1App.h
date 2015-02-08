#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "p2RefBool.h"
#include "j1Timer.h"
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

class j1App
{
public:

	// Constructor
	j1App(const char*);

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

	// Add a new module to handle
	void AddModule(j1Module* module);

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

private:

	bool				quitting;
	j1Timer				ms_timer;
	j1Timer				fps_timer;
	p2List<j1Module*>	modules;
};

#endif