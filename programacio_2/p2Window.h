#ifndef __P2WINDOW_H__
    #define __P2WINDOW_H__

#include "p2Module.h"
#include "SDL\include\SDL.h"

class p2Window : public p2Module
{
public:

	p2Window();

    // Destructor
    virtual ~p2Window();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

private:
	//The window we'll be rendering to
	SDL_Window* window;
	
	//The surface contained by the window
	SDL_Surface* screenSurface;
};

#endif // __P2WINDOW_H__