#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__


class ISoundManager
{

public:
	enum SM_Err  {
		KO = 0,
		OK = 1
	};

	class SoundId {
	public:
		void * pId;
	};

	virtual SM_Err  Init        ()                            = 0;
	virtual SM_Err  End         ()                            = 0;
	virtual SoundId LoadSound   (const char* filename)        = 0;
	virtual SoundId LoadMusic   (const char* filename)        = 0;
	virtual void    PlaySound   (SoundId snd, int loops = 0)  = 0;
	virtual void    UnloadSound (SoundId snd)                 = 0;
	virtual void    UnloadMusic ()                            = 0;
	virtual void    PlayMusic   (SoundId snd, int loops = -1) = 0;
	virtual void    StopMusic   ()                            = 0;
};

#endif