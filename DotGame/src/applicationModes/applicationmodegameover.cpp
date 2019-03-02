#include "globals.h"
#include "applicationmodegameover.h"
#include "applicationmanager.h"
#include "events.h"
//#include "sys.h"
#include "core.h"
//#include "font.h"
#include "asserts.h"

const float ApplicationModeGameOver::TIME_TO_START_GAME = 5.f;

// *************************************************

ApplicationModeGameOver::ApplicationModeGameOver () :
	mMusicId     (0),
	m_pSprite    (nullptr),
	mTimeElapsed (0.f) {}

// *************************************************

ApplicationModeGameOver::~ApplicationModeGameOver()
{
	if (m_pSprite) g_pWindowManager->ReleaseSprite(m_pSprite);
}

// *************************************************

IdMode ApplicationModeGameOver::GetId()
{
	return AM_GameOver;
}

// *************************************************

void ApplicationModeGameOver::Activate()
{
	GAME_ASSERT(g_pEventManager);

	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);

	g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);
	g_pEventManager->Register(this, IEventManager::EM_Event::Quit);

	g_pWindowManager->Init();

	mTimeElapsed = 0.f;

	/*m_pSprite = g_pWindowManager->RequireSprite(Vec2(SCR_HEIGHT/4.f, 350.f), Vec2(400.f, 56.f), (DATA_FOLDER + "gameover.png").c_str(), false);*/

	/*mMusicId = g_pSoundManager->LoadWav((DATA_FOLDER + "DefenseLine.wav").c_str());
	if (mMusicId && g_pApplicationManager->IsAudioActivated()) g_pSoundManager->PlayMusic(mMusicId);*/
}

// *************************************************

void ApplicationModeGameOver::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	//g_pSoundManager->UnloadWav(mMusicId);
	g_pWindowManager->End();
}

// *************************************************

void ApplicationModeGameOver::ProcessInput()
{
	GAME_ASSERT(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************

void ApplicationModeGameOver::Run(float deltaTime)
{
	mTimeElapsed += deltaTime;
	if (mTimeElapsed >= TIME_TO_START_GAME) 
	{
		StartLevel(Game::GameLevel::Level1);
	}
}

// *************************************************

void ApplicationModeGameOver::Render()
{
	g_pWindowManager->Render();
	/*glClearColor(0.f,0.f, 0.f, 1.f);
	glClear( GL_COLOR_BUFFER_BIT );

	glColor3f(1.f, 1.f, 1.f);
	if (m_pSprite) g_pGraphicsEngine->RenderSprite(m_pSprite);
	FONT_DrawString(vmake(20.f, 300.f), m_pProperties->GetProperty("game_over.press_enter.text").c_str());*/
}

// *************************************************

bool ApplicationModeGameOver::ProcessEvent(const Event& event)
{
	switch (event.GetType()) 
	{
		case IEventManager::EM_Event::MouseClick: 
		{
			const EventMouseClick* eMouseClick = dynamic_cast<const EventMouseClick*>(&event);
			if (EventMouseClick::EMouseButton::Left == eMouseClick->GetMouseButton())
			{
				StartLevel(Game::GameLevel::Level1);
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

void ApplicationModeGameOver::StartLevel(Game::GameLevel level)
{
	g_gameLevel = level;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************

void ApplicationModeGameOver::QuitGame()
{
	gQuit = true;
}