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

	Game();
	~Game();

	void Init ();
	void End  ();
	void Run  (float deltaTime);

	GameLevel GetGameLevel() const;
	void      SetGameLevel(GameLevel level);

	int  GetTileIndexFromScreenPosition(Vec2 pos);
	Vec2 GetPositionFromTileIndex(int tileIndex);
	
private:
	static const int MAX_DOTS;
	static const int SECONDS_SPAWN_DOT;

	bool                  mInitialized;
	bool                  mEnded;
	std::vector<Entity *> mEntities;
	GameLevel             mGameLevel;
	float                 mTimeSinceLastEntity;
	std::set<int>         mTilesOccupied;

	void SpawnDot();
};

#endif
