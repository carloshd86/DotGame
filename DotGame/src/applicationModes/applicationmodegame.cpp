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
	mMusicId(0) {}

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

	mMusicId = g_pSoundManager->LoadWav((DATA_FOLDER + "ArcadeFunk.wav").c_str());
}

// *************************************************

void ApplicationModeGame::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	g_pSoundManager->UnloadWav(mMusicId);
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
	g_pWindowManager->Render();
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

void ApplicationModeGame::QuitGame()
{
	gQuit = true;
}