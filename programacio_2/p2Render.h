#ifndef __P2RENDER_H__
    #define __P2RENDER_H__

#include "SDL/include/SDL.h"
#include "p2Module.h"

class p2Render : public p2Module
{
public:

	p2Render();

    // Destructor
    virtual ~p2Render();

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

public:

	SDL_Renderer*	renderer;

};

#endif // __P2RENDER_H__