#ifndef __j1INIREADER_H__
  #define __j1INIREADER_H__

#include "p2Defs.h"

class j1IniReader
{

public:

			j1IniReader();

	void	SetFile(const char* file_name);
	char*	GetString(const char* section, const char* key, const char* default = NULL);
	int		GetInt(const char* section, const char* key, int default = 0);
	bool	GetBool(const char* section, const char* key, bool default = false);

public:

	char file_name[MID_STR];

};

#endif /*__j1INIREADER_H__*/