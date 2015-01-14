#include <windows.h>
#include <stdio.h>

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start( ap, format );
	vsprintf_s( tmp_string, 4096, format, ap );
	va_end( ap );

	sprintf(tmp_string2, "\n%s %s(%d) : %s", __TIME__,file, line, tmp_string);

	OutputDebugString(tmp_string2);
}