#ifndef __j1FILESYSTEM_H__
#define __j1FILESYSTEM_H__

#include "j1Module.h"

struct PHYSFS_file;

class file
{
	char name[256];
	unsigned long GetSize() const;

private:
	PHYSFS_file* f;
};

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

	// Utility functions
	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file) const;

private:

};

#endif // __j1FILESYSTEM_H__