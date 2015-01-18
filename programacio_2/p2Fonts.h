#ifndef __P2FONTS_H__
    #define __P2FONTS_H__

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include "p2Module.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

class p2Fonts : public p2Module
{
public:

	p2Fonts();

    // Destructor
    virtual ~p2Fonts();

    // Called before render is available
    bool Awake();

    // Called before quitting
    bool CleanUp();

	// Load Font
	TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Surface* Print(const char* text, SDL_Color color);

public:

	p2list<TTF_Font *>	fonts;
	TTF_Font*			default;
};


#endif // __P2FONTS_H__