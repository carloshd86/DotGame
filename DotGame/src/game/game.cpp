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
//#include "componentrenderable.h"
#include <ctime>

const int Game::MAX_ENTITIES         = 5;
const int Game::SECONDS_SPAWN_ENTITY = 3;

// *************************************************

Game::Game() : 
	mInitialized         (false), 
	mEnded               (false),
	mGameLevel           (Level1),
	mTimeSinceLastEntity (0.f) {}

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
		g_pWindowManager->SetBackgroundColor(1.f, 1.f, 1.f);

		uint32_t seed = static_cast<uint32_t>(time(0));
		srand(seed);

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
	if (mEntities.size() < MAX_ENTITIES)
	{	
		mTimeSinceLastEntity += deltaTime; 
		if (static_cast<int>(mTimeSinceLastEntity) >= SECONDS_SPAWN_ENTITY)
		{
			mTimeSinceLastEntity = 0.f;
			int newTileIndex = 0;
			do {
				newTileIndex = rand() % (NUM_COLS * NUM_ROWS);
			} while (mTilesOccupied.find(newTileIndex) != mTilesOccupied.end());

			// TODO get position from tileIndex
			// TODO spawn new entity with random color		
		}
	}

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

// *************************************************

int Game::GetTileIndexFromScreenPosition(Vec2 pos)
{
	int tileIndex = -1;

	if (pos.x >= GRID_POS_X && pos.x <= GRID_POS_X + FRAME_WIDTH * NUM_COLS
		&& pos.y >= GRID_POS_Y && pos.y <= GRID_POS_Y + FRAME_HEIGHT * NUM_ROWS)
	{
		for (int j = 0; j < NUM_ROWS; j++) {
			if (pos.y > GRID_POS_Y + FRAME_HEIGHT * j && pos.y < GRID_POS_Y + FRAME_HEIGHT * (j + 1))
			{
				for (int i = 0; i < NUM_COLS; i++) {
					if (pos.x > GRID_POS_X + FRAME_WIDTH * i && pos.x < GRID_POS_X + FRAME_WIDTH * (i + 1))
					{
						tileIndex = NUM_COLS * j + i;
						break;
					}
				}
				break;
			}
		}
	}

	return tileIndex;
}