#include "p2Defs.h"
#include "p2Log.h"
#include "p2App.h"
#include "p2Audio.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

p2Audio::p2Audio() : p2Module()
{
	music = NULL;
	strncpy(name, "audio", SHORT_STR);
}

// Destructor
p2Audio::~p2Audio()
{
}

// Called before render is available
bool p2Audio::Awake()
{
	LOG("Loading Audio Mixer");
	bool ret = true;

	SDL_Init(0);
	if( SDL_InitSubSystem( SDL_INIT_AUDIO ) < 0 )
	{
		LOG( "SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError() );
		ret = false;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if( init & flags != flags) 
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		ret = false;
	}

	//Initialize SDL_mixer
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		LOG( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		ret = false;
	}

	return ret;
}

// Called before quitting
bool p2Audio::CleanUp()
{
	LOG("Freeing sound FX, closing Mixer and Audio subsystem");
	
	if(music!= NULL)
		Mix_FreeMusic(music);

	p2list_item<Mix_Chunk*>* item;

	for(item = fx.start; item != NULL; item = item->next)
	{
		Mix_FreeChunk(item->data);
	}
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool p2Audio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if(music != NULL)
	{
		if(fade_time > 0.0f)
			Mix_FadeOutMusic(fade_time * 1000.0f);
		else
			Mix_HaltMusic();

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}	

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else 
	{
		if(fade_time > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, fade_time * 1000.0f) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
int p2Audio::LoadFx(const char* path)
{
	int ret = -1;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.add(chunk);
		ret = fx.count() - 1;
	}

	return ret;
}

// Play WAV
bool p2Audio::PlayFx(int id, int repeat)
{
	bool ret = false;

	if(id >= 0 && id < fx.count())
	{
		Mix_PlayChannel(-1, fx[id], repeat);
	}

	return ret;
}