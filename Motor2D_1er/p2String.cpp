/**
* String class
*/

#include <stdio.h>
#include <string.h>
#include "p2Assert.h"
#include "p2String.h"

char p2String::tmp[HUGE_STR];

// Constructor

p2String::p2String()
{
	reset();
	alloc(SHORT_STR);
	clear();
}

// Destructor
p2String::~p2String()
{
	destroy();
}

// Reset alloc values
void p2String::reset()
{
	size = max_size = 0;
	string = NULL;
}

// clean p2String
void p2String::clear()
{
	if(string != NULL)
	{
		memset(string, 0, max_size);
	}

	size = 0;
}

/**
* Destroy directly
*/
void p2String::destroy()
{
	RELEASE_ARRAY(string);
	reset();
}

/**
* constructor from another p2String
*/
p2String::p2String(const p2String& str)
{
	reset();
	reserve(str.size);
	memcpy(string, str.string, str.size + 1);
	size = str.size;
}

/**
* Constructor with minimum allocation space
*/
p2String::p2String(unsigned int required_space)
{
	reset();
	reserve(required_space);
	clear();
}

/**
* constructor from formatted char*
*/
p2String::p2String(const char* format, ...)
{
	ASSERT(format != NULL);
	reset();
	va_list ap;
	va_start(ap, format);
	int len = _vsnprintf_s(tmp, HUGE_STR, _TRUNCATE, format, ap);
	va_end(ap);
	ASSERT(len != -1);
	reserve(len);
	memcpy(string, tmp, len + 1);
	size = len;
}

/**
* constructor from formatted char* with max size
*/
p2String::p2String(unsigned int max_size, const char* format, ...)
{
	ASSERT(format != NULL);
	ASSERT(max_size < HUGE_STR);
	reset();
	va_list ap;
	va_start(ap, format);
	_vsnprintf_s(tmp, HUGE_STR, max_size, format, ap);
	va_end(ap);
	tmp[max_size] = '\0';
	int len = strlen(tmp);
	reserve(len);
	memcpy(string, tmp, len + 1);
	size = len;
}

/**
* create from another p2String
*/
p2String& p2String::create(const p2String& str)
{
	if(&str != this)
	{
		if(str.length() > 0)
		{
			reserve(str.length());
			memcpy(string, (const char*)str, str.length() + 1);
			size = str.length();
		}
		else
		{
			clear();
		}
	}

	return(*this);
}

/**
* create from formatted char*
*/
p2String& p2String::create(const char* format, ...)
{
	if(format != NULL)
	{
		va_list ap;
		va_start(ap, format);
		int len = _vsnprintf_s(tmp, HUGE_STR, _TRUNCATE, format, ap);
		va_end(ap);
		ASSERT(len != -1);
		reserve(len);
		memcpy(string, tmp, len + 1);
		size = len;
	}
	else
	{
		clear();
	}

	return(*this);
}

/**
* create from formatted char* with max size
*/
p2String& p2String::create(unsigned int max_size, const char* format, ...)
{
	ASSERT(max_size < HUGE_STR);

	if(format != NULL)
	{
		va_list ap;
		va_start(ap, format);
		_vsnprintf_s(tmp, HUGE_STR, max_size, format, ap);
		va_end(ap);
		tmp[max_size] = '\0';
		int len = strlen(tmp);
		reserve(len);
		memcpy(string, tmp, len + 1);
		size = len;
	}
	else
	{
		clear();
	}

	return(*this);
}

/**
* Update calculate members. Used when other processes modify the data directly.
*/
void p2String::update()
{
	size = strlen(string);
}

/**
* Make sure that "alloc space" is allocated
*/
void p2String::reserve(const unsigned int required_space)
{
	if((required_space + 1) <= max_size)
	{
		return;
	}

	alloc(required_space + 1);
}

/**
* check for proper size
*/
unsigned int p2String::get_reserved() const
{
	return(max_size);
}

/**
* check for proper size
*/
void p2String::make_room(const unsigned int required_space)
{
	if((size + required_space + 1) < max_size)
	{
		return;
	}

	if(required_space < SHORT_STR)
	{
		alloc(max_size + SHORT_STR);
	}
	else
	{
		alloc(max_size + required_space + SHORT_STR);
	}
}

// Allocate memory space, it will destroy current string
void p2String::alloc(unsigned int new_size)
{
	bool saved_str = false;

	if(string != NULL)
	{
		ASSERT_MSG(size < HUGE_STR, "Not enough room for temporal string");
		memcpy(tmp, string, size + 1);
		saved_str = true;
	}
	else
		destroy();

	string = new char[new_size];
	max_size = new_size;
	clear();

	if(saved_str == true)
	{
		memcpy(string, tmp, MIN(new_size - 1, strlen(tmp)));
	}

	// realloc ?
}

/**
* get a character from the p2String (const)
*/
const char p2String::operator [] (int index) const
{
	if(index < 0 || index >(int) size)
	{
		return(0);
	}

	return(string[index]);
}

/**
* get a character from the p2String
*/
char* p2String::operator  [] (int index)
{
	if(string != NULL)
	{
		ASSERT(index >= 0 && index < (int)max_size);
		return(&string[index]);
	}
	else
	{
		return(0);
	}
}

/**
* Cuts the string
*/
void p2String::cut(unsigned int start, unsigned int end)
{
	if(string != NULL)
	{
		start = MIN(start, size);
		end = (end == 0) ? size : MIN(end, size);
		ASSERT(start <= end);
		int s = end - start;

		if(start > 0)
		{
			memmove(string, &string[start], s);
		}

		string[s] = '\0';
		update();
	}
}

/**
* Paste a substring into buffer
*/
unsigned int p2String::sub_string(unsigned int start, unsigned int end, char* buffer) const
{
	if(string != NULL)
	{
		ASSERT(buffer != NULL);
		start = MIN(start, size);
		end = (end == 0) ? size : MIN(end, size);
		ASSERT(start <= end);
		int s = end - start;
		memcpy(buffer, &string[start], s);
		buffer[s] = '\0';
		return(end - start);
	}
	else
	{
		buffer[0] = '\0';
		return(0);
	}
}

/**
* Paste a substring into buffer
*/
unsigned int p2String::sub_string(unsigned int start, unsigned int end, p2String& buffer) const
{
	if(string != NULL)
	{
		start = MIN(start, size);
		end = (end == 0) ? size : MIN(end, size);
		ASSERT(start <= end);
		int s = end - start;
		buffer.create(s, "%s", &(string[start]));
		return(end - start);
	}
	else
	{
		return(0);
	}
}

/**
* Convert the string to upper case
*/
p2String& p2String::to_upper_case()
{
	if(string != NULL)
	{
		for(register unsigned int i = 0; i < size; ++i)
		{
			string[i] = toupper(string[i]);
		}
	}

	return(*this);
}

/**
* Convert the string to lower case
*/
p2String& p2String::to_lower_case()
{
	if(string != NULL)
	{
		for(register unsigned int i = 0; i < size; ++i)
		{
			string[i] = tolower(string[i]);
		}
	}

	return(*this);
}

/**
* Convert all to lower case but the first char
*/
p2String& p2String::capitalize()
{
	if(string != NULL)
	{
		for(register unsigned int i = 0; i < size; ++i)
		{
			if(i == 0)
			{
				string[i] = toupper(string[i]);
			}
			else
			{
				string[i] = tolower(string[i]);
			}
		}
	}

	return(*this);
}

/**
* Cut spaces (or other char) from left/right of the string
*/
p2String& p2String::trim(int left, int right, char ch)
{
	if(string != NULL)
	{
		unsigned int  pos_left = find_char(ch, 0);
		unsigned int  pos_right = find_char(ch, 1);

		if(left)
		{
			if(pos_left > 0)
			{
				memcpy(string, &string[pos_left], size - pos_left);
				string[size - pos_left] = 0;
			}
		}

		if(right)
		{
			if(pos_right < size - 1)
			{
				string[pos_right - pos_left + 1] = '\0';
			}
		}

		update();
	}

	return(*this);
}

/**
* Locates the first or last entry of a char
*/
unsigned int p2String::find_char(char ch, int from_last)
{
	unsigned int  i = 0;

	if(string != NULL)
	{
		if(from_last == 1)
		{
			i = size - 1;
		}

		while(i < size && i >= 0)
		{
			if(string[i] != ch)
			{
				return(i);
			}

			if(from_last == 0)
			{
				++i;
			}
			else
			{
				--i;
			}
		}
	}

	return(i);
}

/**
* Count all entries of certain char
*/
unsigned int p2String::count_char(char ch)
{
	unsigned int  iRet = 0;

	if(string != NULL)
	{
		for(unsigned int u = 0; u < length(); ++u)
		{
			if(string[u] == ch)
			{
				iRet++;
			}
		}
	}

	return(iRet);
}

/**
* Returns a token from a list separated with "separator" ... like "a:b:dsadf:asd:12:token"
*/
int p2String::get_token(const char* separator, unsigned int num_token, p2String& buffer) const
{
	ASSERT(separator != NULL);
	return(0);
}

/**
* Remove a char
*/
p2String& p2String::delete_char(char ch)
{
	if(string != NULL)
	{
		int bEliminarChar = 1;
		int iCopy = 0;

		for(register unsigned int i = 0; i < size; ++i)
		{
			if(string[i] != ch)
			{
				string[iCopy] = (string[i]);
				iCopy++;
			}
		}

		string[iCopy] = '\0';
		update();
	}

	return(*this);
}


/**
* copy from another p2String
*/
p2String& p2String::operator=(const p2String& str)
{
	create("%s", str.string);
	return(*this);
}

/**
* copy from char*
*/
p2String& p2String::operator=(const char* str)
{
	if(str != NULL)
	{
		create("%s", str);
	}
	else
	{
		clear();
	}

	return(*this);
}

/**
* Concat another p2String
*/
p2String p2String::operator+(const p2String& str) const
{
	p2String  str1(*this);
	return(str1 += str);
}

/**
* Concat another char*
*/
p2String p2String::operator+(const char* str) const
{
	p2String  str1(*this);
	return(str1 += str);
}

/**
* Concat char* + string
*/
p2String operator+(const char* str1, const p2String& str2)
{
	p2String  str("%s%s", str1, str2);
	return(str);
}

/**
* Concat directle from an p2String
*/
p2String& p2String::operator+=(const p2String& str)
{
	make_room(str.size + 1);
	memcpy((string + size), str.string, str.size + 1);
	size += str.size;
	return(*this);
}

/**
* copy from another p2String
*/
p2String& p2String::operator+=(const char* str)
{
	if(str != NULL)
	{
		unsigned int  len = (unsigned int)strlen(str);
		make_room(len + 1);
		memcpy((string + size), str, len + 1);
		size += len;
	}

	return(*this);
}

/**
* Concat a single char
*/
p2String& p2String::operator+=(const char ch)
{
	make_room(1);
	string[size] = ch;
	string[size + 1] = 0;
	size += 1;
	return(*this);
}

/**
* Add formatted text
*/
p2String& p2String::add(const char* format, ...)
{
	ASSERT(format != NULL);
	va_list ap;
	va_start(ap, format);
	int len = _vsnprintf_s(tmp, HUGE_STR, HUGE_STR, format, ap);
	va_end(ap);
	ASSERT(len != -1);
	make_room(len + 1);
	memcpy((string + size), tmp, len + 1);
	size += len;
	return(*this);
}

/**
* copy from another CString
*/
p2String& p2String::prepend(const char* format, ...)
{
	ASSERT(format != NULL);
	va_list ap;
	va_start(ap, format);
	int len = _vsnprintf_s(tmp, HUGE_STR, HUGE_STR, format, ap);
	va_end(ap);
	ASSERT(len != -1);
	make_room(len + 1);
	memcpy((string + len), string, size + 1);
	memcpy(string, tmp, len);
	size += len;
	return(*this);
}


/**
* Compare (case sensitive) 2 p2Strings
*/
int p2String::operator==(const p2String& str) const
{
	if(size != str.size)
	{
		return(0);
	}

	if(strcmp(string, str.string) != 0)
	{
		return(0);
	}

	return(1);
}

/**
* Compare (case sensitive) a p2String with a char*
*/
int p2String::operator==(const char* str) const
{
	ASSERT(str != NULL);

	if(size != strlen(str))
	{
		return(0);
	}

	if(strcmp(string, str) != 0)
	{
		return(1);
	}

	return(1);
}

/**
* Compare (case sensitive) 2 p2Strings
*/
int p2String::operator!=(const p2String& str) const
{
	return(!(*this == str));
}

/**
* Compare (case sensitive) a p2String with a char*
*/
int p2String::operator!=(const char* str) const
{
	return(!(*this == str));
}

/**
* Compare (case INsensitive) 2 p2Strings
*/
bool p2String::compare_no_case(const p2String& str) const
{
	if(size != str.size)
	{
		return(false);
	}

	if(_stricmp(string, str.string) != 0)
	{
		return(false);
	}

	return(true);
}

/**
* Compare (case INsensitive) a p2String with a char*
*/
bool p2String::compare_no_case(const char* str) const
{
	ASSERT(str != NULL);

	if(size != strlen(str))
	{
		return(false);
	}

	if(_stricmp(string, str) != 0)
	{
		return(false);
	}

	return(true);
}
