#ifndef __P2_STRING__
#define __P2_STRING__

#include "p2Defs.h"

class p2String
{

private:

	char*         string;
	unsigned int  max_size;
	unsigned int  size;
	static char   tmp[HUGE_STR];

public:

	p2String();
	~p2String();

	// constructors -----------------------------------------------------------
	p2String(unsigned int required_space);
	p2String(const p2String& str);
	p2String(const char* format, ...);
	p2String(unsigned int max_size, const char* format, ...);
	p2String&   create(const p2String& str);
	p2String&   create(const char* format, ...);
	p2String&   create(unsigned int max_size, const char* format, ...);

	// utils -----------------------------------------------------------
	void          destroy();
	void          update();
	void          clear();
	void          reserve(const unsigned int required_space);
	unsigned int  get_reserved() const;
	void          cut(unsigned int start, unsigned int end);
	unsigned int  sub_string(unsigned int start, unsigned int end, char* buffer) const;
	unsigned int  sub_string(unsigned int start, unsigned int end, p2String& buffer) const;
	p2String&   to_upper_case();
	p2String&   to_lower_case();
	p2String&   capitalize();
	p2String&   capitalize_after_underscores();
	p2String&   trim(int left = true, int right = true, char ch = ' ');
	p2String&   delete_char(char ch);
	unsigned int  find_char(char ch, int from_last = false);
	unsigned int  count_char(char ch);
	int           get_token(const char* separator, unsigned int num_token, p2String& buffer) const;

	/*$1- Fast util ----------------------------------------------------------*/
	inline int is_empty() const
	{
		return(size == 0);
	}

	inline unsigned int length() const
	{
		return(size);
	}

	inline char* c_str()
	{
		return(string);
	}

	inline const char* c_str() const
	{
		return(string);
	}

	inline void update_size()
	{
		size = strlen(string);
	}

	inline char get_char(int pos)
	{
		return(string[pos]);
	}

	inline char get_char(int pos) const
	{
		return(string[pos]);
	}

	// Operators ----------------------------------------------------------
	p2String& operator                =(const p2String& str);
	p2String& operator                =(const char* str);

	p2String operator                 +(const p2String& str) const;
	p2String operator                 +(const char* str) const;

	friend p2String operator +(const char* str1, const p2String& str2);

	p2String& operator                +=(const p2String& str);
	p2String& operator                +=(const char* str);
	p2String& operator                +=(const char ch);

	int operator                        ==(const p2String& str) const;
	int operator                        ==(const char* str) const;

	int operator                        !=(const p2String& str) const;
	int operator                        !=(const char* str) const;

	const char operator [](int index) const;
	char* operator  [](int index);

	// Other concats ------------------------------------------------------
	p2String&   add(const char* format, ...);
	p2String&   prepend(const char* format, ...);

	// cast operators -----------------------------------------------------
	inline operator const char*  () const
	{
		return(string);
	}

	inline operator char*  ()
	{
		return(string);
	}

	// special comparisions -----------------------------------------------
	bool compare_no_case(const p2String& str) const;
	bool compare_no_case(const char* str) const;

private:

	// internal utils -----------------------------------------------------
	void  make_room(const unsigned int required_space);
	void  alloc(unsigned int size);
	void  reset();

};

#endif // __P2_STRING__