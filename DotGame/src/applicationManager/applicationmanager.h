#ifndef __APPLICATION_MANAGER_H__
#define __APPLICATION_MANAGER_H__


#include "applicationmode.h"
#include "properties.h"


class ApplicationManager
{
public:

	ApplicationManager();
	~ApplicationManager();

	void SwitchMode        (IdMode mode);
	void ManageModeChange  ();
	void ProcessInput      ();
	void Run               (float deltaTime);
	void Render            ();

	Properties::P_Language GetLang           () const;
	void                   SetLang           (Properties::P_Language lang);
	bool                   IsAudioActivated  () const;
	void                   SetAudioActivated (bool audioActivated);

	
private:

	IdMode                  mDesiredMode;
	ApplicationMode        *m_pGameMode;
	Properties::P_Language  mLang;
	bool                    mAudioActivated;
};

#endif
