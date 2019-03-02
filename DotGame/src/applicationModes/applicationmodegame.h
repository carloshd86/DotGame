#ifndef __APPLICATION_MODE_GAME_H__
#define __APPLICATION_MODE_GAME_H__

#include "applicationmode.h"

class ApplicationModeGame : public ApplicationMode, public IEventManager::IListener
{
public:
	ApplicationModeGame();
	~ApplicationModeGame();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();

	bool ProcessEvent(const Event& event);

private:
	Properties                                *m_pProperties;
	unsigned int                               mMusicId;

	void QuitGame();
};

#endif