#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

p2Textures::p2Textures() : p2Module()
{
	strncpy(name, "textures", SHORT_STR);
}

// Destructor
p2Textures::~p2Textures()
{
}

// Called before render is available
bool p2Textures::Awake()
{
	LOG("Init Image library");
	bool ret = true;
	 	
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if( init & flags != flags) 
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool p2Textures::Start()
{
	LOG("start textures");
	bool ret = true;

	return ret;
}

// Called each loop iteration
bool p2Textures::PreUpdate()
{
	return true;
}

bool p2Textures::Update(float dt)
{
	
	return true;
}


bool p2Textures::PostUpdate()
{
	return true;
}

// Called before quitting
bool p2Textures::CleanUp()
{
	LOG("Freeing textures and Image library");

	p2list_item<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		SDL_DestroyTexture(item->data);
	}

	textures.clear();

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const p2Textures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
        SDL_FreeSurface( surface );
	}

	return texture;
}

// Translate a surface into a texture
SDL_Texture* const p2Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface( App->render->renderer, surface );

    if( texture == NULL )
    {
        LOG( "Unable to create texture from surface! SDL Error: %s\n", SDL_GetError() );
    }
	else
	{
		textures.add(texture);
	}

	return texture;
}
