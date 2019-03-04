#ifndef __SDL_SOUND_MANAGER_H__
#define __SDL_SOUND_MANAGER_H__

#include "soundmanager.h"
#include <SDL_mixer.h>
#include <set>

class SdlSoundManager : public ISoundManager
{

public:
	~SdlSoundManager();

	static SdlSoundManager * Instance();

	SM_Err  Init        ();
	SM_Err  End         ();
	SoundId LoadSound   (const char* filename);
	SoundId LoadMusic   (const char* filename);
	void    PlaySound   (SoundId snd, int loops = 0);
	void    UnloadSound (SoundId snd);
	void    UnloadMusic ();
	void    PlayMusic   (SoundId snd, int loops = -1);
	void    StopMusic   ();

private:
	SdlSoundManager();

	static SdlSoundManager *mInstance;

	bool                 mInitialized;
	bool                 mEnded;
	std::set<Mix_Chunk*> mLoadedSounds;
	Mix_Music*           mLoadedMusic;
};

#endif