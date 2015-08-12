#include "p2Defs.h"
#include "j1App.h"
#include "p2Log.h"
#include "j1Serialization.h"

j1Serialization::j1Serialization() : j1Module()
{
	name.create("serialization");
}

// Destructor
j1Serialization::~j1Serialization()
{
}

// Called before render is available
bool j1Serialization::Awake(j1IniReader* conf)
{
	LOG("Loading Serialization System");
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Serialization::CleanUp()
{
	LOG("Freeing Serialization subsystem");

	return true;
}

// Load a game state from a file ---------------------
bool j1Serialization::LoadGameState(const char* file)
{
	bool ret = false;

	char* buffer;
	uint size = App->fs->Load(file, &buffer);

	if(size > 0)
	{
		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if(result != NULL)
		{
			LOG("Loading new Game State from %s...", file);
			App->LoadGameState();
			data.reset();
			LOG("...finished loading");
			ret = true;
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", file, result.description());
	}
	else
		LOG("Could not load game state xml file %s", file);

	return ret;
}

// Load a game state from a file ---------------------
bool j1Serialization::SetLoadSection(const char* section)
{
	bool ret = false;

	if(data.empty() == false)
	{
		this->section = data.child(section);
		if(this->section != NULL)
			ret = true;
	}

	return ret;
}

// Load an int from current section 
int j1Serialization::LoadInt(const char* name, int default)
{
	int ret = default;

	if(section != NULL)
	{
		pugi::xml_attribute attr = section.attribute(name);

		if(attr != NULL)
			ret = attr.as_int(default);
	}

	return ret;
}

// Load an bool from current section 
bool j1Serialization::LoadBool(const char* name, bool default)
{
	bool ret = default;

	if(section != NULL)
	{
		pugi::xml_attribute attr = section.attribute(name);

		if(attr != NULL)
			ret = attr.as_bool(default);
	}

	return ret;
}

// Load an float from current section 
float j1Serialization::LoadFloat(const char* name, float default)
{
	float ret = default;

	if(section != NULL)
	{
		pugi::xml_attribute attr = section.attribute(name);

		if(attr != NULL)
			ret = attr.as_float(default);
	}

	return ret;
}

// Load an c string from current section 
const char* j1Serialization::LoadString(const char* name, const char* default)
{
	const char* ret = default;

	if(section != NULL)
	{
		pugi::xml_attribute attr = section.attribute(name);

		if(attr != NULL)
			ret = attr.as_string(default);
	}

	return ret;
}

// Convenience function for pugi::xml to dump data
void j1Serialization::write(const void* data, size_t size)
{
	if(save_buffer_point + size < SAVE_BUFFER_SIZE)
	{
		memcpy(save_buffer + save_buffer_point, data, size);
		save_buffer_point += size;
	}
	else
	{
		LOG("Buffer overrun while saving game state!");
		save_buffer_point = SAVE_BUFFER_SIZE;
	}
}

// Load a game state from a file ---------------------
bool j1Serialization::SaveGameState(const char* file)
{
	bool ret = false;

	LOG("Saving  Game State to %s...", file);

	// xml object were we will store all data
	data.reset();

	ret = App->SaveGameState();

	if(ret == true)
	{
		save_buffer_point = 0;
		data.save(*this);

		// we are done, so write data to disk
		if(save_buffer_point > 0 && save_buffer_point < SAVE_BUFFER_SIZE)
		{
			App->fs->Save(file, save_buffer, save_buffer_point);
			LOG("... finished saving", file);
		}
	}

	return ret;
}

// Set a write section ---------------------
void j1Serialization::SetWriteSection(const char* name)
{
	if(data.empty() == false)
	{
		section = data.append_child(name);
	}
}

// Save a value to current xml ---------------
void j1Serialization::SaveInt(const char* name, int value)
{
	if(section != NULL)
	{
		pugi::xml_attribute attr = section.append_attribute(name);

		if(attr != NULL)
			attr.set_value(value);
	}
}

// Save a value to current xml ---------------
void j1Serialization::SaveBool(const char* name, bool value)
{
	if(section != NULL)
	{
		pugi::xml_attribute attr = section.append_attribute(name);

		if(attr != NULL)
			attr.set_value(value);
	}
}

// Save a value to current xml ---------------
void j1Serialization::SaveFloat(const char* name, float value)
{
	if(section != NULL)
	{
		pugi::xml_attribute attr = section.append_attribute(name);

		if(attr != NULL)
			attr.set_value(value);
	}
}

// Save a value to current xml ---------------
void j1Serialization::SaveString(const char* name, const char* value)
{
	if(section != NULL)
	{
		pugi::xml_attribute attr = section.append_attribute(name);

		if(attr != NULL)
			attr.set_value(value);
	}
}
