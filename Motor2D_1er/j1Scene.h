#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before the first frame
	bool Start(j1IniReader* conf);

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	p2String map;
	SDL_Texture* debug_tex;
	SDL_Texture* test_gui;
};

#endif // __j1SCENE_H__