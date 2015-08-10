#include "p2Assert.h"
#include "j1IniReader.h"
#include "IniParser\iniparser.h"

// ---------------------------------------------
j1IniReader::j1IniReader() : dict(NULL)
{
	section[0] = '\0';
}

j1IniReader::~j1IniReader()
{
	if(dict != NULL)
		iniparser_freedict(dict);
}

// ---------------------------------------------
void j1IniReader::SetBuffer(const char* buffer, unsigned int lenght)
{
	ASSERT(file);

	if(dict != NULL)
		iniparser_freedict(dict);

	dict = iniparser_load_buffer(buffer, lenght);
}

// ---------------------------------------------
bool j1IniReader::SetSection(const char* section)
{
	ASSERT(section);
	
	sprintf_s(tmp, 80, "%s", section);

	if(iniparser_find_entry(dict, tmp) == 1)
	{
		strcpy_s(this->section, 80, section);
		return true;
	}

	return false;
}

// ---------------------------------------------
const char* j1IniReader::GetString(const char* key, char* default)
{
	sprintf_s(tmp, 80, "%s:%s", section, key);
	return iniparser_getstring(dict, tmp, default);
}

// ---------------------------------------------
int j1IniReader::GetInt(const char* key, int default)
{
	sprintf_s(tmp, 80, "%s:%s", section, key);
	return iniparser_getint(dict, tmp, default);
}

// ---------------------------------------------
bool j1IniReader::GetBool(const char* key, bool default)
{
	sprintf_s(tmp, 80, "%s:%s", section, key);
	return iniparser_getboolean(dict, tmp, default) != 0;
}