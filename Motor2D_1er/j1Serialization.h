#ifndef __j1SERIALIZATION_H__
#define __j1SERIALIZATION_H__

#include "j1Module.h"

#define SAVE_BUFFER_SIZE 2048

class j1Serialization : public j1Module, public pugi::xml_writer
{
public:

	j1Serialization();

	// Destructor
	virtual ~j1Serialization();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before quitting
	bool CleanUp();

	// Utility functions
	bool LoadGameState(const char* file);
	bool SaveGameState(const char* file);

	// Load functions
	bool SetLoadSection(const char* section);

	int			LoadInt(const char* name, int default = 0);
	bool		LoadBool(const char* name, bool default = false);
	float		LoadFloat(const char* name, float default = 0.0f);
	const char*	LoadString(const char* name, const char* default = "");

	// Save function
	// Convenience function for pugi::xml to dump data
	void SetWriteSection(const char* name);
	void write(const void* data, size_t size);

	void	SaveInt(const char* name, int value);
	void	SaveBool(const char* name, bool value);
	void	SaveFloat(const char* name, float value);
	void	SaveString(const char* name, const char* value);

private:

	pugi::xml_document data;
	pugi::xml_node section;
	char save_buffer[SAVE_BUFFER_SIZE];
	unsigned int save_buffer_point;
};

#endif // __j1SERIALIZATION_H__