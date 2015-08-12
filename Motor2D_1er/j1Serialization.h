#ifndef __j1SERIALIZATION_H__
#define __j1SERIALIZATION_H__

#include "j1Module.h"

class j1Serialization : public j1Module
{
public:

	j1Serialization();

	// Destructor
	virtual ~j1Serialization();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before quitting
	bool CleanUp();

private:

};

#endif // __j1SERIALIZATION_H__