#ifndef __GAME_H__
#define __GAME_H__

#include "core.h"
#include "windowmanager.h"
#include <vector>


class Entity;
class GameInputManager;

class Game
{
public:

	enum GameLevel
	{
		Level1,
		Level2,
		Level3
	};

	static const GameLevel FINAL_GAME_LEVEL = Level3;

	Game();
	~Game();

	void Init ();
	void End  ();
	void Run  (float deltaTime);

	Entity           * GetPlayer       () const;
	GameLevel          GetGameLevel    () const;
	void               SetGameLevel    (GameLevel level);
	
	static GameLevel GetNextLevel(GameLevel level);

private:
	bool                   mInitialized;
	bool                   mEnded;
	std::vector<Entity *>  mEntities;
	Entity                *m_pPlayer;
	std::vector<Entity *>  mWalls;
	std::vector<Entity *>  mEnemies;
	Entity                *m_pGoal;
	GameLevel              mGameLevel;
};

#endif
