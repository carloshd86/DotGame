#include "globals.h"
#include "applicationmodegame.h"
#include "applicationmanager.h"
#include "game.h"
#include "windowmanager.h"
//#include "sys.h"
#include "core.h"
//#include "font.h"
#include "events.h"
#include "asserts.h"
#include "memorycontrol.h"


ApplicationModeGame::ApplicationModeGame () :
	mMusicId(0),
	m_pProperties(nullptr),
	mLastTileIndexHover(-1) {}

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

	g_pGame->Init();
	g_pWindowManager->Init();

	std::string tileBackground = DATA_FOLDER + "TileBackground.png";
	for (int j = 0; j < NUM_ROWS; j++) {
		for (int i = 0; i < NUM_COLS; i++) {
			mTilesMap[NUM_COLS * j + i] = g_pWindowManager->RequireSprite(Vec2(GRID_POS_X + FRAME_WIDTH * i, GRID_POS_Y + FRAME_HEIGHT * j), Vec2(FRAME_HEIGHT, FRAME_WIDTH), tileBackground.c_str());
		}
	}

	//mMusicId = g_pSoundManager->LoadWav((DATA_FOLDER + "ArcadeFunk.wav").c_str());
}

// *************************************************

void ApplicationModeGame::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	mTilesMap.clear();

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	//g_pSoundManager->UnloadWav(mMusicId);
	g_pWindowManager->End();
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
	g_pWindowManager->SetColor(1.f, 1.f, 1.f, 1.f);
	g_pWindowManager->Render();

	g_pWindowManager->SetColor(0.f, 0.f, 0.f, 1.f);

	for (int i = 0; i <= NUM_ROWS; i++)
	{
		float newY = GRID_POS_Y + FRAME_HEIGHT * i;
		g_pWindowManager->DrawLine(GRID_POS_X, newY, GRID_POS_X + FRAME_WIDTH * NUM_COLS, newY);
	}
	for (int i = 0; i <= NUM_COLS; i++)
	{
		float newX = GRID_POS_X + FRAME_WIDTH * i;
		g_pWindowManager->DrawLine(newX, GRID_POS_Y, newX, GRID_POS_Y + FRAME_HEIGHT * NUM_ROWS);
	}
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
	case IEventManager::EM_Event::MouseClick: 
	{
		const EventMouseClick* eMouseClick = dynamic_cast<const EventMouseClick*>(&event);
		if (EventMouseClick::EMouseButton::Left == eMouseClick->GetMouseButton())
		{
			// TODO check if mouse position is green circle
		} else if (EventMouseClick::EMouseButton::Right == eMouseClick->GetMouseButton())
		{
			// TODO check if mouse position is red circle
		}
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
	int tileIndex = g_pGame->GetTileIndexFromScreenPosition(pos);

	if (mLastTileIndexHover != tileIndex)
	{
		if (mLastTileIndexHover >= 0)
		{
			g_pWindowManager->ReleaseSprite(mTilesMap[mLastTileIndexHover]);
			std::string tileBackground = DATA_FOLDER + "TileBackground.png";
			int i = mLastTileIndexHover % NUM_COLS;
			int j = mLastTileIndexHover / NUM_COLS;
			mTilesMap[mLastTileIndexHover] = g_pWindowManager->RequireSprite(Vec2(GRID_POS_X + FRAME_WIDTH * i, GRID_POS_Y + FRAME_HEIGHT * j), Vec2(FRAME_HEIGHT, FRAME_WIDTH), tileBackground.c_str());
			mLastTileIndexHover = -1;
		}

		if (tileIndex >= 0)
		{
			g_pWindowManager->ReleaseSprite(mTilesMap[tileIndex]);
			std::string tileBackground = DATA_FOLDER + "TileMouseOver.png";
			int i = tileIndex % NUM_COLS;
			int j = tileIndex / NUM_COLS;
			mTilesMap[tileIndex] = g_pWindowManager->RequireSprite(Vec2(GRID_POS_X + FRAME_WIDTH * i, GRID_POS_Y + FRAME_HEIGHT * j), Vec2(FRAME_HEIGHT, FRAME_WIDTH), tileBackground.c_str());
			mLastTileIndexHover = tileIndex;
		}
	}
}
