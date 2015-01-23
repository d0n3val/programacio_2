#ifndef __P2DEFS_H__
    #define __P2DEFS_H__

#include <stdio.h>
#include "p2Assert.h"

//  NULL just in case ----------------------

  #ifdef NULL
    #undef NULL
  #endif
  #define NULL  0

// Deletes a buffer 
  #define RELEASE( x ) \
  {                        \
    if( x != NULL )        \
    {                      \
      delete x;            \
	  x = NULL;              \
    }                      \
  }

// Deletes an array of buffers
  #define RELEASE_ARRAY( x ) \
  {                              \
    if( x != NULL )              \
    {                            \
      delete[] x;                \
	  x = NULL;                    \
    }                            \
                              \
  }

// Standard string size
#define SHORT_STR	32
#define MID_STR		255
#define HUGE_STR	8192

// Joins a path and file
inline const char* const PATH(const char* folder, const char *file)
{
	static char path[MID_STR];
	sprintf_s(path, MID_STR, "%s/%s", folder, file);
	return path;
}

#endif