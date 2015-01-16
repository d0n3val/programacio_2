#ifndef __P2INPUT_H__
    #define __P2INPUT_H__

#include "p2Module.h"
#include "SDL\include\SDL.h"

enum p2EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

class p2Input : public p2Module
{
public:

	p2Input();

    // Destructor
    virtual ~p2Input();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PreUpdate();
	bool Update(float dt);

    // Called before quitting
    bool CleanUp();

	bool GetWindowEvent(p2EventWindow ev);

	/*
	// Check key states (includes mouse and joy buttons)
	bool GetKeypDown(int code);
	bool GetKeypRepeat(int code);
	bool GetKeypUp(int code);

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	bool GetMousePosition(int &x, int &y);
	*/

private:
	void CleanKeys();

private:
	bool windowEvents[WE_COUNT];
};

#endif // __P2INPUT_H__