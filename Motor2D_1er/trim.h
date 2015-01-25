
#ifndef TRIM_H
#define TRIM_H

#include <string.h>

#ifndef STRRSPN
/*
  A non-standard helper function:
  Find c such that s=".*c[d]*", where [d]=delimiters and c in [d].
  If c is not found, it returns the argument s.
*/
const char* strrspn( const char* s, const char* delimiters )
{
  const char* p = strchr( s, '\0' );
  while (p-- != s)
    if (!strchr( delimiters, *p ))
      break;
  return p + 1;
}
#endif

#ifndef STRNCPY0
#define _CRT_SECURE_NO_WARNINGS
/*
  A non-standard helper function:
  A strncpy() that guarantees termination of the result string.
  The result string must be at minimum n+1 characters long.
*/
char* strncpy0( char* result, const char* s, size_t n )
{
  strncpy_s( result, n, s, n );
  result[ n ] = '\0';
  return result;
}
#endif 
/*
char* trim_right_copy( char* result, const char* s, const char* delimiters )
{
  return strncpy0( result, s, strrspn( s, delimiters ) - s );
}

char* trim_left_copy( char* result, const char* s, const char* delimiters )
{
  return strcpy( result, s + strspn( s, delimiters ) );
}

char* trim_copy( char* result, const char* s, const char* delimiters )
{
  trim_left_copy( result, s, delimiters );
  result[ strrspn( result, delimiters ) - result ] = '\0';
  return result;
}
*/
char* trim_right_inplace( char* s, const char* delimiters )
{
  s[ strrspn( s, delimiters ) - s ] = '\0';
  return s;
}

char* trim_left_inplace( char* s, const char* delimiters )
{
  char* p = s + strspn( s, delimiters );
  return (char *) memmove( s, p, strlen( p ) + 1 );
}

char* trim_inplace( char* s, const char* delimiters )
{
  return trim_left_inplace( trim_right_inplace( s, delimiters ), delimiters );
}

#endif