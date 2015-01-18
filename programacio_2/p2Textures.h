#ifndef __P2TEXTURES_H__
    #define __P2TEXTURES_H__

#include "SDL/include/SDL.h"
#include "p2Module.h"

class p2Textures : public p2Module
{
public:

	p2Textures();

    // Destructor
    virtual ~p2Textures();

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

	p2list<SDL_Texture*>	textures;

	SDL_Texture*	example;
};


#endif // __P2TEXTURES_H__