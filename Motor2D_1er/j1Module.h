// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __j1MODULE_H__
#define __j1MODULE_H__

#include "p2String.h"

class j1App;
class j1IniReader;
class j1Serialization;
class Gui;

class j1Module
{
public:

	j1Module() : active(false), update_on_pause(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(j1IniReader* conf)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start(j1IniReader* conf)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// Listener to gui events
	virtual void OnGui(Gui* gui, int type)
	{
	}

	// Called to load a game state
	virtual bool LoadGameState(j1Serialization* serial)
	{
		return true;
	}

	// Called to save a game state
	virtual bool SaveGameState(j1Serialization* serial)
	{
		return true;
	}

public:

	p2String	name;
	bool		active;
	bool		update_on_pause;

};

#endif // __j1MODULE_H__