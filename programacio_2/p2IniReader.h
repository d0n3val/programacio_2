#ifndef __P2INIREADER_H__
  #define __P2INIREADER_H__

#include "p2Defs.h"

class p2IniReader
{

public:

			p2IniReader();

	void	SetFile(const char* file_name);
	char*	GetString(const char* section, const char* key, const char* default = NULL);
	int		GetInt(const char* section, const char* key, int default = 0);

public:

	char file_name[255];

};

#endif /*__P2INIREADER_H__*/