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
#include "componentrenderable.h"
#include "componentdot.h"
#include <ctime>

const int    Game::MAX_DOTS          = 5;
const int    Game::SECONDS_SPAWN_DOT = 3;
const size_t Game::SCORE_WIN         = 10;

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
	if (mEntities.size() < MAX_DOTS)
	{	
		mTimeSinceLastEntity += deltaTime; 
		if (static_cast<int>(mTimeSinceLastEntity) >= SECONDS_SPAWN_DOT)
		{
			mTimeSinceLastEntity = 0.f;
			SpawnDot();
		}
	}

	for (auto entity : mEntities) entity->Run(deltaTime);

	DeleteEntities();
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

// *************************************************

Vec2 Game::GetPositionFromTileIndex(int tileIndex)
{
	Vec2 pos;

	int i = tileIndex % NUM_COLS;
	int j = tileIndex / NUM_COLS;
	pos.x = GRID_POS_X + FRAME_WIDTH * i;
	pos.y = GRID_POS_Y + FRAME_HEIGHT * j;

	return pos;
}

// *************************************************

void Game::SpawnDot()
{
	int dotTile = 0;
	do {
		dotTile = rand() % (NUM_COLS * NUM_ROWS);
	} while (mTilesOccupied.find(dotTile) != mTilesOccupied.end());

	Vec2 dotPos = GetPositionFromTileIndex(dotTile);

	std::string dotRedImg   = DATA_FOLDER + "ClickableRed.png";
	std::string dotGreenImg = DATA_FOLDER + "ClickableGreen.png";

	const char* dotImage  = nullptr;
	DotType dotType = DotType::Green;

	int iDotType = rand() % 2;
	if (iDotType) 
	{
		dotImage = dotGreenImg.c_str();
		dotType  = DotType::Green;
	}
	else
	{
		dotImage = dotRedImg.c_str();
		dotType  = DotType::Red;
	}

	Entity* dotEntity        = GAME_NEW(Entity, (dotPos));
	CRenderable* cRenderable = GAME_NEW(CRenderable, (dotEntity, Vec2(FRAME_WIDTH, FRAME_HEIGHT), dotImage));
	cRenderable->Init();
	CDot * cDot              = GAME_NEW(CDot, (dotEntity, dotType, dotTile));
	cDot->Init();
	
	dotEntity->AddComponent(cRenderable);
	dotEntity->AddComponent(cDot);

	mTilesOccupied.insert(dotTile);
	mEntities.push_back(dotEntity);
}

// *************************************************

void Game::DeleteEntities()
{
	size_t numEntitiesToDelete = mEntitiesToDelete.size();
	if (numEntitiesToDelete)
	{
		auto etdEnd = mEntitiesToDelete.end();
		auto it = mEntities.begin();
		while (numEntitiesToDelete > 0 && it != mEntities.end())
		{
			if (mEntitiesToDelete.find(*it) != etdEnd)
			{
				RequireTileMessage rtm;
				(*it)->ReceiveMessage(rtm);
				GAME_ASSERT(rtm.GetProcessed());
				mTilesOccupied.erase(mTilesOccupied.find(rtm.GetTile()));

				RequireSpriteMessage rsm;
				(*it)->ReceiveMessage(rsm);
				GAME_ASSERT(rsm.GetProcessed());
				g_pWindowManager->ReleaseSprite(rsm.GetSprite());

				it = mEntities.erase(it);
				numEntitiesToDelete--;
			}
			else
			{
				++it;
			}
		}

		mEntitiesToDelete.clear();
	}
}

// *************************************************

void Game::IncreaseScore(Entity* entityHit)
{
	RequireDotTypeMessage rdtm;
	entityHit->ReceiveMessage(rdtm);
	GAME_ASSERT(rdtm.GetProcessed());
	mScore.push_back(rdtm.GetDotType());

	mEntitiesToDelete.insert(entityHit);

	if (mScore.size() >= SCORE_WIN) SetGameOver(GameResult::Success);
}

// *************************************************

void Game::SetGameOver(GameResult result)
{
	if (result == Success) gGameSuccess = true;
	else                   gGameSuccess = false;

	g_pApplicationManager->SwitchMode(AM_GameOver);
}

// *************************************************

size_t Game::GetFinalScore() const
{
	return mScore.size();
}