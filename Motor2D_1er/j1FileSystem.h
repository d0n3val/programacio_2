#ifndef __j1FILESYSTEM_H__
#define __j1FILESYSTEM_H__

#include "j1Module.h"

class j1FileSystem : public j1Module
{
public:

	j1FileSystem();

	// Destructor
	virtual ~j1FileSystem();

	// Called before render is available
	bool Awake();

	// Called before quitting
	bool CleanUp();

private:

};

#endif // __j1FILESYSTEM_H__