#ifndef __P2INPUT_H__
    #define __P2INPUT_H__

#include "p2Module.h"
#include "SDL/include/SDL.h"

#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5

enum p2EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum p2KeyState
{
	KS_IDLE = 0,
	KS_DOWN,
	KS_REPEAT,
	KS_UP
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

    // Called before quitting
    bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(p2EventWindow ev);

	// Check key states (includes mouse and joy buttons)
	bool GetKeyDown(int code);
	bool GetKeyRepeat(int code);
	bool GetKeyUp(int code);

	// Check if a certain window event happened
	bool GetWindowEvent(int code);

	// Get mouse / axis position
	//bool GetMousePosition(int &x, int &y);
	void GetMouseMotion(int &x, int &y);

	bool GetMouseButtonDown(int code);
	bool GetMouseButtonRepeat(int code);
	bool GetMouseButtonUp(int code);

private:
	void CleanKeys();

private:
	bool		windowEvents[WE_COUNT];
	p2KeyState	keyState[NUM_KEYS];
	p2KeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x;
	int			mouse_motion_y;
};

#endif // __P2INPUT_H__