#ifndef __P2DEFS_H__
    #define __P2DEFS_H__

// Memory Leaks reporter -------------------
#define _CRTDBG_MAP_ALLOC

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
    }                      \
                        \
    x = NULL;              \
  }

// Deletes an array of buffers
  #define RELEASE_ARRAY( x ) \
  {                              \
    if( x != NULL )              \
    {                            \
      delete[] x;                \
    }                            \
                              \
    x = NULL;                    \
  }

#endif