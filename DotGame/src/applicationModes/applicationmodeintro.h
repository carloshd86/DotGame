#ifndef __APPLICATION_MODE_MENU_H__
#define __APPLICATION_MODE_MENU_H__

#include "applicationmode.h"

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
	static const float TIME_TO_START_GAME;

	Properties *m_pProperties;
	uint32_t    mMusicId;
	float       mTimeElapsed;
	int         mRestingTime;
	std::string mTitleText;
	std::string mStartText;
	std::string mAutomaticStartText;

	void StartLevel(Game::GameLevel level);
	void QuitGame  ();
};

#endif