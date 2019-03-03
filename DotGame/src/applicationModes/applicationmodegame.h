#ifndef __APPLICATION_MODE_GAME_H__
#define __APPLICATION_MODE_GAME_H__

#include "applicationmode.h"
#include "game.h"

class ApplicationModeGame : public ApplicationMode, public IEventManager::IListener, public Game::IScoreListener
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
	bool ScoreAdd    (Game::DotType dotType);

private:
	Properties*                m_pProperties;
	uint32_t                   mMusicId;
	std::map<int, ISprite*>    mTilesMap;
	int32_t                    mLastTileIndexHover;
	std::vector<ISprite*>      mScoreSprites;

	void CheckTileMouseHover(Vec2 pos);
	void QuitGame();
};

#endif