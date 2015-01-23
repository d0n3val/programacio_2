#include <Windows.h>
#include <string.h>
#include "j1IniReader.h"

j1IniReader::j1IniReader()
{
	memset(file_name, 0, 255);
}

// ---------------------------------------------
void j1IniReader::SetFile(const char* file)
{
	strncpy_s(file_name, file, 256);
}

// ---------------------------------------------
char* j1IniReader::GetString(const char* section, const char* key, const char* default)
{
	static char result[MID_STR];

	memset(result, 0, MID_STR);

	GetPrivateProfileString(section, key, default, result, MID_STR, file_name); 

	return result;
}

// ---------------------------------------------
int j1IniReader::GetInt(const char* section, const char* key, int default)
{
	return GetPrivateProfileInt(section, key, default, file_name);
}

// ---------------------------------------------
bool j1IniReader::GetBool(const char* section, const char* key, bool default)
{
	char* result = GetString(section, key, "false");

	if(_stricmp(result, "true") == 0)
		return true;

	if(_stricmp(result, "false") == 0)
		return false;

	return default;
}