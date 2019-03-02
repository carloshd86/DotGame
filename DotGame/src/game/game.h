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
		Level1
	};

	Game();
	~Game();

	void Init ();
	void End  ();
	void Run  (float deltaTime);

	GameLevel          GetGameLevel    () const;
	void               SetGameLevel    (GameLevel level);
	
private:
	bool                   mInitialized;
	bool                   mEnded;
	std::vector<Entity *>  mEntities;
	GameLevel              mGameLevel;
};

#endif
