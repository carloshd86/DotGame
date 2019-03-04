#include "globals.h"
#include "applicationmodegame.h"
#include "applicationmanager.h"
#include "game.h"
#include "windowmanager.h"
#include "core.h"
#include "events.h"
#include "asserts.h"
#include "memorycontrol.h"


ApplicationModeGame::ApplicationModeGame() :
	m_pProperties(nullptr),
	mLastTileIndexHover(-1)
{
	mMusicId.pId     = nullptr;
}

// *************************************************

ApplicationModeGame::~ApplicationModeGame()
{
}

// *************************************************

IdMode ApplicationModeGame::GetId()
{
	return AM_Game;
}

// *************************************************

void ApplicationModeGame::Activate()
{
	GAME_ASSERT(g_pEventManager);

	// Properties
	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);

	// Register events for this 
	g_pEventManager->Register(this, IEventManager::EM_Event::MouseMove);
	g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);
	g_pEventManager->Register(this, IEventManager::EM_Event::Quit);

	// Game
	if (!g_pGame) g_pGame = GAME_NEW(Game, ());
	g_pGame->SetGameLevel(g_gameLevel);

	gFinalScore = 0;
	g_pGame->Init();
	g_pGame->Register(this);
	g_pWindowManager->InitWindow();

	std::string tileBackground = DATA_FOLDER + "TileBackground.png";
	Vec2 frameSize(FRAME_HEIGHT, FRAME_WIDTH);
	for (int j = 0; j < NUM_ROWS; j++) {
		for (int i = 0; i < NUM_COLS; i++) {
			float posX = static_cast<float>(GRID_POS_X + FRAME_WIDTH * i);
			float posY = static_cast<float>(GRID_POS_Y + FRAME_HEIGHT * j);
			mTilesMap[NUM_COLS * j + i] = g_pWindowManager->RequireSprite(Vec2(posX, posY), frameSize, tileBackground.c_str(), 0);
		}
	}

	mMusicId     = g_pSoundManager->LoadMusic("ArcadeFunk.ogg");
	if (mMusicId.pId && g_pApplicationManager->IsAudioActivated()) g_pSoundManager->PlayMusic(mMusicId);
}

// *************************************************

void ApplicationModeGame::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	mTilesMap.clear();
	mScoreSprites.clear();

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	g_pSoundManager->StopMusic();
	g_pSoundManager->UnloadMusic();
	g_pWindowManager->EndWindow();
	g_pGame->Unregister(this);
	g_pGame->End();
}

// *************************************************

void ApplicationModeGame::ProcessInput()
{
	GAME_ASSERT(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************

void ApplicationModeGame::Run(float deltaTime)
{
	g_pGame->Run(deltaTime);
}

// *************************************************

void ApplicationModeGame::Render()
{
	g_pWindowManager->ClearColorBuffer(1.f, 1.f, 1.f);
	g_pWindowManager->Render();

	g_pWindowManager->SetColor(0.f, 0.f, 0.f, 1.f);

	for (int i = 0; i <= NUM_ROWS; i++)
	{
		float newY = static_cast<float>(GRID_POS_Y + FRAME_HEIGHT * i);
		g_pWindowManager->DrawLine(static_cast<float>(GRID_POS_X), newY, static_cast<float>(GRID_POS_X + FRAME_WIDTH * NUM_COLS), newY);
	}
	for (int i = 0; i <= NUM_COLS; i++)
	{
		float newX = static_cast<float>(GRID_POS_X + FRAME_WIDTH * i);
		g_pWindowManager->DrawLine(newX, static_cast<float>(GRID_POS_Y), newX, static_cast<float>(GRID_POS_Y + FRAME_HEIGHT * NUM_ROWS));
	}

	g_pWindowManager->RefreshRendering();
}

// *************************************************

bool ApplicationModeGame::ProcessEvent(const Event& event) 
{
	switch (event.GetType()) 
	{
	case IEventManager::EM_Event::MouseMove: 
	{
		const EventMouseMove* eMouseMove = dynamic_cast<const EventMouseMove*>(&event);
		gMouseX = eMouseMove->GetPosX();
		gMouseY = eMouseMove->GetPosY();
		CheckTileMouseHover(Vec2(gMouseX, gMouseY));
		break;
	}
	case IEventManager::EM_Event::Quit: 
	{
		QuitGame();
		break;
	}
	}

	return true;
}

// *************************************************

void ApplicationModeGame::QuitGame()
{
	gQuit = true;
}

// *************************************************

void ApplicationModeGame::CheckTileMouseHover(Vec2 pos)
{
	int tileIndex = Game::GetTileIndexFromScreenPosition(pos);

	if (mLastTileIndexHover != tileIndex)
	{
		if (mLastTileIndexHover >= 0)
		{
			g_pWindowManager->ReleaseSprite(mTilesMap[mLastTileIndexHover]);
			std::string tileBackground = DATA_FOLDER + "TileBackground.png";
			int i = mLastTileIndexHover % NUM_COLS;
			int j = mLastTileIndexHover / NUM_COLS;
			float posX = static_cast<float>(GRID_POS_X + FRAME_WIDTH * i);
			float posY = static_cast<float>(GRID_POS_Y + FRAME_HEIGHT * j);
			mTilesMap[mLastTileIndexHover] = g_pWindowManager->RequireSprite(Vec2(posX, posY), Vec2(FRAME_HEIGHT, FRAME_WIDTH), tileBackground.c_str(), 0);
			mLastTileIndexHover = -1;
		}

		if (tileIndex >= 0)
		{
			g_pWindowManager->ReleaseSprite(mTilesMap[tileIndex]);
			std::string tileBackground = DATA_FOLDER + "TileMouseOver.png";
			int i = tileIndex % NUM_COLS;
			int j = tileIndex / NUM_COLS;
			float posX = static_cast<float>(GRID_POS_X + FRAME_WIDTH * i);
			float posY = static_cast<float>(GRID_POS_Y + FRAME_HEIGHT * j);
			mTilesMap[tileIndex] = g_pWindowManager->RequireSprite(Vec2(posX, posY), Vec2(FRAME_HEIGHT, FRAME_WIDTH), tileBackground.c_str(), 0);
			mLastTileIndexHover = tileIndex;
		}
	}
}

// *************************************************

bool ApplicationModeGame::ScoreAdd(Game::DotType dotType)
{
	Vec2 scoreEntryPos(SCORE_POS_X + FRAME_WIDTH * mScoreSprites.size(), SCORE_POS_Y);

	std::string image;
	if (Game::DotType::Green == dotType) image = DATA_FOLDER + "ClickableGreen.png";
	else                                 image = DATA_FOLDER + "ClickableRed.png";

	ISprite* newSprite = g_pWindowManager->RequireSprite(scoreEntryPos, Vec2(FRAME_WIDTH, FRAME_HEIGHT), image.c_str());
	mScoreSprites.push_back(newSprite);

	return true;
}