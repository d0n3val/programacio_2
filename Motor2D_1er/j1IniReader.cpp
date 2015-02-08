#include <Windows.h>
#include <string.h>
#include "p2Assert.h"
#include "j1IniReader.h"

// ---------------------------------------------
j1IniReader::j1IniReader() : tmp(MID_STR)
{}

// ---------------------------------------------
void j1IniReader::SetFile(const char* file)
{
	ASSERT(file);

	file_name = file;
}

// ---------------------------------------------
const p2String& j1IniReader::GetString(const char* section, const char* key, const char* default)
{
	GetPrivateProfileString(section, key, default, tmp.c_str(), MID_STR, file_name);
	tmp.update();

	return tmp;
}

// ---------------------------------------------
int j1IniReader::GetInt(const char* section, const char* key, int default)
{
	return GetPrivateProfileInt(section, key, default, file_name);
}

// ---------------------------------------------
bool j1IniReader::GetBool(const char* section, const char* key, bool default)
{
	GetString(section, key, "false");

	if(tmp.compare_no_case("true"))
	{
		return true;
	}

	if(tmp.compare_no_case("false"))
	{
		return false;
	}

	return default;
}