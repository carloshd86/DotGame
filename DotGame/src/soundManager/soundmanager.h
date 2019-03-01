#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__


class ISoundManager
{

public:

	virtual bool         InitSound ()                                            = 0;
	virtual void         EndSound  ()                                            = 0;
	virtual unsigned int LoadWav   (const char filename[])                       = 0;
	virtual void         PlaySound (unsigned int snd, float volume, float pitch) = 0;
	virtual void         UnloadWav (unsigned int snd)                            = 0;
	virtual void         PlayMusic (unsigned int snd, float volume = 1.f)        = 0;
	virtual void         StopMusic ()                                            = 0;
};

#endif