#ifndef __P2AUDIO_H__
    #define __P2AUDIO_H__

#include "p2Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

class p2Audio : public p2Module
{
public:

	p2Audio();

    // Destructor
    virtual ~p2Audio();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

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

#endif // __P2AUDIO_H__