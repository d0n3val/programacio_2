#ifndef __j1RENDER_H__
    #define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "j1Module.h"

class j1Render : public j1Module
{
public:

	j1Render();

    // Destructor
    virtual ~j1Render();

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

	// Blit
	bool Blit(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Color		background;
};

#endif // __j1RENDER_H__