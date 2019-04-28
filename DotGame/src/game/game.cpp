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

const int    Game::MAX_DOTS                     = 8;
const int    Game::SECONDS_SPAWN_DOT            = 1;
const size_t Game::SCORE_WIN                    = 10;
const int    Game::MIN_SECONDS_CHANGE_PHASE_DOT = 1;
const int    Game::MAX_SECONDS_CHANGE_PHASE_DOT = 5;

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
	int dotTile = ObtainNewFreeTile();
	Vec2 dotPos = Game::GetPositionFromTileIndex(dotTile);

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
	int timePhaseChange      = MIN_SECONDS_CHANGE_PHASE_DOT + (rand() % (MAX_SECONDS_CHANGE_PHASE_DOT - MIN_SECONDS_CHANGE_PHASE_DOT + 1));
	CDot * cDot              = GAME_NEW(CDot, (dotEntity, dotType, dotTile, static_cast<float>(timePhaseChange)));
	cDot->Init();
	
	dotEntity->AddComponent(cRenderable);
	dotEntity->AddComponent(cDot);

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
				ReleaseTile(rtm.GetTile());

				RequireSpriteMessage rsm;
				(*it)->ReceiveMessage(rsm);
				GAME_ASSERT(rsm.GetProcessed());

				GAME_DELETE(*it);
				it = mEntities.erase(it);

				g_pWindowManager->ReleaseSprite(rsm.GetSprite());

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
	gFinalScore++;
	RequireDotTypeMessage rdtm;
	entityHit->ReceiveMessage(rdtm);
	GAME_ASSERT(rdtm.GetProcessed());

	for (auto it = mScoreListeners.begin(); it != mScoreListeners.end(); ++it)
	{
		(*it)->ScoreAdd(rdtm.GetDotType());
	}

	mEntitiesToDelete.insert(entityHit);

	if (gFinalScore >= SCORE_WIN) SetGameOver(GameResult::Success);
}

// *************************************************

void Game::SetGameOver(GameResult result)
{
	if (result == Success) gGameSuccess = true;
	else                   gGameSuccess = false;

	g_pApplicationManager->SwitchMode(AM_GameOver);
}

// *************************************************

void Game::ReleaseTile(int tile)
{
	if (mTilesOccupied.find(tile) != mTilesOccupied.end())
		mTilesOccupied.erase(mTilesOccupied.find(tile));
}

// *************************************************

int Game::ObtainNewFreeTile()
{
	int freeTile = 0;
	do {
		freeTile = rand() % (NUM_COLS * NUM_ROWS);
	} while (mTilesOccupied.find(freeTile) != mTilesOccupied.end());

	mTilesOccupied.insert(freeTile);

	return freeTile;
}

// *************************************************

bool Game::Register(IScoreListener* listener)
{
	mScoreListeners.push_back(listener);
	return true;
}

// *************************************************

bool Game::Unregister(IScoreListener* listener)
{
	for (auto it = mScoreListeners.begin(); it != mScoreListeners.end(); ++it)
	{
		if (*it == listener)
		{
			it = mScoreListeners.erase(it);
			break;
		}
	}
	return true;
}