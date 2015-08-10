#ifndef __j1INIREADER_H__
#define __j1INIREADER_H__

#include "p2Defs.h"
#include "p2String.h"

struct dictionary;

class j1IniReader
{
public:

	j1IniReader();
	~j1IniReader();

	bool	SetSection(const char* section);
	void	SetBuffer(const char* buffer, unsigned int lenght);
	const char*	GetString(const char* key, char* default = NULL);
	int		GetInt(const char* key, int default = 0);
	bool	GetBool(const char* key, bool default = false);

private:

	dictionary* dict;
	char section[80];
	char tmp[80];
};

#endif /*__j1INIREADER_H__*/