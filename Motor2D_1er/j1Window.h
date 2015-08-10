#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"
#include "SDL/include/SDL.h"

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PreUpdate();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	p2String	title;
	p2String	final_title;
};

#endif // __j1WINDOW_H__