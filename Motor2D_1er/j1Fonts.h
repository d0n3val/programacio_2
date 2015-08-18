#ifndef __j1FONTS_H__
#define __j1FONTS_H__

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include "j1Module.h"

#define DEFAULT_FONT "fonts/open_sans/OpenSans-Regular.ttf"
#define DEFAULT_FONT_SIZE 12

class j1Fonts : public j1Module
{
public:

	j1Fonts();

	// Destructor
	virtual ~j1Fonts();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before quitting
	bool CleanUp();

	// Load Font
	TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = {255, 255, 255, 255}, TTF_Font* font = NULL);

public:

	p2List<TTF_Font*>	fonts;
	TTF_Font*			default;
};


#endif // __j1FONTS_H__