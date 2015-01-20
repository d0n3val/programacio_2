#ifndef __P2WINDOW_H__
    #define __P2WINDOW_H__

#include "p2Module.h"
#include "SDL/include/SDL.h"

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
	char	title[MID_STR]; 
};

#endif // __P2WINDOW_H__