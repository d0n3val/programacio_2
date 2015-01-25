#ifndef __j1TEXTURES_H__
#define __j1TEXTURES_H__

#include "SDL/include/SDL.h"
#include "j1Module.h"

class j1Textures : public j1Module
{
public:

	j1Textures();

	// Destructor
	virtual ~j1Textures();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load Texture
	SDL_Texture* const Load(const char* path);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);

public:

	p2List<SDL_Texture*>	textures;

	SDL_Texture*	example;
};


#endif // __j1TEXTURES_H__