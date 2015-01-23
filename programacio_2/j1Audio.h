#ifndef __j1AUDIO_H__
    #define __j1AUDIO_H__

#include "j1Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class j1Audio : public j1Module
{
public:

	j1Audio();

    // Destructor
    virtual ~j1Audio();

    // Called before render is available
    bool Awake();

    // Called before quitting
    bool CleanUp();

	// Play a music file 
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(int fx, int repeat = 0);

private:

	Mix_Music*			music;
	p2list<Mix_Chunk*>	fx;
};

#endif // __j1AUDIO_H__