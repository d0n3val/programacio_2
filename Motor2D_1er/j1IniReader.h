#ifndef __j1INIREADER_H__
#define __j1INIREADER_H__

#include "p2Defs.h"
#include "p2String.h"

class j1IniReader
{

public:

	j1IniReader();

	void		SetFile(const char* file_name);
	const p2String&	GetString(const char* section, const char* key, const char* default = NULL);
	int			GetInt(const char* section, const char* key, int default = 0);
	bool		GetBool(const char* section, const char* key, bool default = false);

public:

	p2String file_name;
	p2String tmp;

};

#endif /*__j1INIREADER_H__*/