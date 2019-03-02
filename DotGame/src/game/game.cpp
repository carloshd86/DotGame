#include "globals.h"
#include "game.h"
#include "applicationmanager.h"
#include "entity.h"
#include "messagereceiver.h"
#include "messages.h"
#include "asserts.h"
#include "memorycontrol.h"
#include <fstream>
#include "component.h"
//#include "componentenemy.h"
//#include "componentgoal.h"
//#include "componentmovable.h"
//#include "componentplayer.h"
//#include "componentrenderable.h"
//#include "componentrigidbody.h"
//#include "componentroutepath.h"


Game::Game() : 
	mInitialized     (false), 
	mEnded           (false) {}

// *************************************************

Game::~Game()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************

void Game::Init()
{
	if (!mInitialized)
	{
		// Init game state
		
		mInitialized = true;
	}
}

// *************************************************

void Game::End()
{
	if (mInitialized)
	{
		for(auto entity : mEntities) { GAME_DELETE(entity); }
		mEntities.clear();

		mInitialized = false;
	}
	mEnded = true;
}

// *************************************************

void Game::Run(float deltaTime)
{
	for (auto entity : mEntities) entity->Run(deltaTime);
}

// *************************************************

Game::GameLevel Game::GetGameLevel() const 
{
	return mGameLevel;
}

// *************************************************

void Game::SetGameLevel(GameLevel level)
{
	mGameLevel = level;
}