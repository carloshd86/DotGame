#ifndef __APPLICATION_MODE_MENU_H__
#define __APPLICATION_MODE_MENU_H__

#include "applicationmode.h"
#include "game.h"

class ApplicationModeIntro : public ApplicationMode, public IEventManager::IListener
{
public:
	ApplicationModeIntro();
	~ApplicationModeIntro();

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
	ISprite                                   *m_pSprite;

	void StartLevel           (Game::GameLevel level);
	void QuitGame             ();
};

#endif