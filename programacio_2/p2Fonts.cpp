#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Fonts.h"

#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

p2Fonts::p2Fonts() : p2Module()
{
	strncpy(name, "fonts", SHORT_STR);
}

// Destructor
p2Fonts::~p2Fonts()
{
}

// Called before render is available
bool p2Fonts::Awake()
{
	LOG("Init True Type Font library");
	bool ret = true;
	 	
	if( TTF_Init() == -1 )
    {
		LOG( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		ret = false;
    }
	else
	{
		const char* path = App->config.GetString("fonts", "default_font", DEFAULT_FONT);
		int size = App->config.GetInt("fonts", "default_size", DEFAULT_FONT_SIZE);
	
		default = Load(path, size);
	}

	return ret;
}

// Called before quitting
bool p2Fonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");

	p2list_item<TTF_Font*>* item;

	for(item = fonts.start; item != NULL; item = item->next)
	{
		TTF_CloseFont(item->data);
	}

	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const p2Fonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if(font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.add(font);
	}

	return font;
}

// Print text using font
SDL_Texture* p2Fonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{
	SDL_Texture* ret = NULL;

	SDL_Surface* surface = TTF_RenderText_Solid( (font) ? font : default, text, color );

	if(surface == NULL)
	{
		LOG( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface( surface );
	}

	return ret;
}