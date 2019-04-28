#include "sdlsoundmanager.h"
#include "globals.h"
#include "memorycontrol.h"
#include "asserts.h"

SdlSoundManager * SdlSoundManager::mInstance = nullptr;

// *************************************************

SdlSoundManager::SdlSoundManager() :
	mInitialized (false),
	mEnded       (false),
	mLoadedMusic (nullptr)
{
}

// *************************************************

SdlSoundManager::~SdlSoundManager()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************

SdlSoundManager * SdlSoundManager::Instance()
{
	if (!mInstance) {
		mInstance = GAME_NEW(SdlSoundManager, ());
	}

	return mInstance;
}

// *************************************************

ISoundManager::SM_Err SdlSoundManager::Init()
{
	if (!mInitialized)
	{
		GAME_ASSERT(g_pWindowManager);

		if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1 )
		{
			printf("Mix_OpenAudio: Failed to init! %s\n", Mix_GetError());
			return KO;    
		}

		int flags = MIX_INIT_OGG;
		if ((Mix_Init(flags) & flags) != flags) 
		{
			printf("Mix_Init: Failed to init required ogg support! %s\n", Mix_GetError());
			return KO;
		}
	}
	return OK;
}

// *************************************************

ISoundManager::SM_Err SdlSoundManager::End()
{
	if (!mEnded)
	{
		for(Mix_Chunk* chunk : mLoadedSounds) Mix_FreeChunk(chunk);
		mLoadedSounds.clear();

		if (mLoadedMusic) Mix_FreeMusic(mLoadedMusic);

		Mix_CloseAudio();
		Mix_Quit();
	}
	return OK;
}

// *************************************************

ISoundManager::SoundId SdlSoundManager::LoadSound(const char* filename)
{
	SoundId soundId;
	Mix_Chunk* chunk = Mix_LoadWAV((DATA_FOLDER + filename).c_str());	
	soundId.pId = chunk;
	mLoadedSounds.insert(chunk);
	return soundId;
}

// *************************************************

ISoundManager::SoundId SdlSoundManager::LoadMusic(const char* filename)
{
	SoundId soundId;
	mLoadedMusic = Mix_LoadMUS((DATA_FOLDER + filename).c_str());	
	soundId.pId = mLoadedMusic;
	return soundId;
}

// *************************************************

void SdlSoundManager::PlaySound(SoundId snd, int loops)
{
	Mix_Chunk* chunk = static_cast<Mix_Chunk*>(snd.pId);
	if (chunk) Mix_PlayChannel(-1, chunk, loops);
}

// *************************************************

void SdlSoundManager::UnloadSound(SoundId snd)
{
	auto sound = mLoadedSounds.find(static_cast<Mix_Chunk*>(snd.pId));
	if (sound != mLoadedSounds.end())
	{
		Mix_FreeChunk(*sound);
		mLoadedSounds.erase(sound);
	}
}

// *************************************************

void SdlSoundManager::UnloadMusic()
{
	if (mLoadedMusic)
	{
		Mix_FreeMusic(mLoadedMusic);
		mLoadedMusic = nullptr;
	}
}

// *************************************************

void SdlSoundManager::PlayMusic(SoundId snd, int loops)
{
	if (mLoadedMusic)
	{
		Mix_PlayMusic(mLoadedMusic, 1);
	}
}

// *************************************************

void SdlSoundManager::StopMusic()
{
	if (mLoadedMusic && Mix_PlayingMusic())
	{
		Mix_HaltMusic();
	}
}