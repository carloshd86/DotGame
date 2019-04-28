#ifndef __GAME_H__
#define __GAME_H__

#include "core.h"
#include <vector>
#include <set>

class Entity;

class Game
{
public:
	enum GameLevel
	{
		Level1
	};

	enum GameResult
	{
		Success,
		Fail
	};

	enum DotType
	{
		Green,
		Red
	};

	class IScoreListener 
	{
	public:
		virtual bool ScoreAdd (DotType dotType) = 0;
	};

	Game();
	~Game();

	void Init ();
	void End  ();
	void Run  (float deltaTime);

	GameLevel GetGameLevel() const;
	void      SetGameLevel(GameLevel level);

	void IncreaseScore     (Entity* entityHit);
	void SetGameOver       (GameResult result);
	void ReleaseTile       (int tile);
	int  ObtainNewFreeTile ();

	bool Register   (IScoreListener* listener);
	bool Unregister (IScoreListener* listener);
	
	static int  GetTileIndexFromScreenPosition (Vec2 pos);
	static Vec2 GetPositionFromTileIndex       (int tileIndex);

private:
	static const int    MAX_DOTS;
	static const int    SECONDS_SPAWN_DOT;
	static const size_t SCORE_WIN;
	static const int    MIN_SECONDS_CHANGE_PHASE_DOT;
	static const int    MAX_SECONDS_CHANGE_PHASE_DOT;

	bool                 mInitialized;
	bool                 mEnded;
	std::vector<Entity*> mEntities;
	GameLevel            mGameLevel;
	float                mTimeSinceLastEntity;
	std::set<int>        mTilesOccupied;
	std::set<Entity*>    mEntitiesToDelete;

	std::vector<IScoreListener*> mScoreListeners;

	void SpawnDot();
	void DeleteEntities();
};

#endif
