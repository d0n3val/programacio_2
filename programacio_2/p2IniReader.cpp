#include <Windows.h>
#include <string.h>
#include "p2IniReader.h"

p2IniReader::p2IniReader()
{
	memset(file_name, 0, 255);
}

// ---------------------------------------------
void p2IniReader::SetFile(const char* file)
{
	strncpy_s(file_name, file, 256);
}

// ---------------------------------------------
char* p2IniReader::GetString(const char* section, const char* key, const char* default)
{
	char* result = new char[MID_STR];

	memset(result, 0, MID_STR);

	GetPrivateProfileString(section, key, default, result, MID_STR, file_name); 

	return result;
}

// ---------------------------------------------
int p2IniReader::GetInt(const char* section, const char* key, int default)
{
	return GetPrivateProfileInt(section, key, default, file_name);
}

// ---------------------------------------------
bool p2IniReader::GetBool(const char* section, const char* key, bool default)
{
	char* result = GetString(section, key, "false");

	if(stricmp(result, "true") == 0)
		return true;

	if(stricmp(result, "false") == 0)
		return false;

	return default;
}