#ifndef __APPLICATION_MODE_GAME_OVER_H__
#define __APPLICATION_MODE_GAME_OVER_H__

#include "applicationmode.h"

class ApplicationModeGameOver : public ApplicationMode, public IEventManager::IListener
{
public:
	ApplicationModeGameOver();
	~ApplicationModeGameOver();

	IdMode GetId           ();
	void   Activate        ();
	void   Deactivate      ();
	void   ProcessInput    ();
	void   Run             (float deltaTime);
	void   Render          ();

	bool ProcessEvent(const Event& event);

private:
	Properties  *m_pProperties;
	unsigned int mMusicId;
	ISprite     *m_pSprite;

	void StartLevel (Game::GameLevel level);
	void QuitGame   ();
};

#endif