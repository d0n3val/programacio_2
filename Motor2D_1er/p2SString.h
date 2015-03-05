#ifndef __p2SString_H__
#define __p2SString_H__

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define TMP_STRING_SIZE	4096

class p2SString
{
private:

	char* str;
	unsigned int size;

public:

	// Constructors
	p2SString()
	{
		Alloc(1);
		Clear();
	}

	p2SString(unsigned int size)
	{
		if(size > 0)
			Alloc(size);
		else
			Alloc(1);

		Clear();
	}

	p2SString(const p2SString& string)
	{
		Alloc(string.size);
		strcpy_s(str, size, string.str);
	}

	p2SString(const char *format, ...)
	{
		size = 0;

		if(format != NULL)
		{
			static char tmp[TMP_STRING_SIZE];
			static va_list  ap;

			// Construct the string from variable arguments
			va_start(ap, format);
			int res = vsprintf_s(tmp, TMP_STRING_SIZE, format, ap);
			va_end(ap);

			if(res > 0)
			{
				Alloc(res + 1);
				strcpy_s(str, size, tmp);
			}
		}

		if(size == 0)
		{
			Alloc(1);
			Clear();
		}
	}
	
	// Destructor
	virtual ~p2SString()
	{
		if(str != NULL)
			delete[] str;
	}

	// Operators
	bool operator== (const p2SString& string) const
	{
		return strcmp(string.str, str) == 0;
	}

	bool operator== (const char* string) const
	{
		if(string != NULL)
			return strcmp(string, str) == 0;
		return false;
	}

	bool operator!= (const p2SString& string) const
	{
		return strcmp(string.str, str) != 0;
	}

	bool operator!= (const char* string) const
	{
		if(string != NULL)
			return strcmp(string, str) != 0;
		return true;
	}
	
	const p2SString& operator= (const p2SString& string)
	{
		if(string.Length() + 1 > size)
		{
			delete[] str;
			Alloc(string.Length() + 1);
		}
		else
			Clear();

		strcpy_s(str, size, string.str);

		return(*this);
	}

	const p2SString& operator= (const char* string)
	{
		if(string != NULL)
		{
			if(strlen(string) + 1 > size)
			{
				delete[] str;
				Alloc(strlen(string)+1);
			}
			else
				Clear();

			strcpy_s(str, size, string);
		}
		else
		{
			Clear();
		}

		return(*this);
	}
	
	const p2SString& operator+= (const p2SString& string)
	{
		static char tmp[TMP_STRING_SIZE];
		strcpy_s(tmp, TMP_STRING_SIZE, str);
		strcat_s(tmp, TMP_STRING_SIZE - strlen(str), string.str);

		if(strlen(tmp) + 1 > size)
		{
			delete[] str;
			Alloc(strlen(tmp)+1);
		}
		else
			Clear();

		strcpy_s(str, size, tmp);

		return(*this);
	}

	const p2SString& operator+= (const char* string)
	{
		static char tmp[TMP_STRING_SIZE];
		strcpy_s(tmp, TMP_STRING_SIZE, str);
		strcat_s(tmp, TMP_STRING_SIZE - strlen(str), string);

		if(strlen(tmp) + 1 > size)
		{
			delete[] str;
			Alloc(strlen(tmp) + 1);
		}
		else
			Clear();

		strcpy_s(str, size, tmp);

		return(*this);
	}

	// Utils
	unsigned int Length() const
	{
		return strlen(str);
	}

	void Clear()
	{
		str[0] = '\0';
	}

	const char* GetString() const
	{
		return str;
	}

	unsigned int GetCapacity() const
	{
		return size;
	}

private:

	void Alloc(unsigned int requiered_memory)
	{
		size = requiered_memory;
		str = new char[size];
	}

};

#endif // __p2SString_H__