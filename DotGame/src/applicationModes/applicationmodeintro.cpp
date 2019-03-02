#include "globals.h"
#include "applicationmodeintro.h"
#include "applicationmanager.h"
#include "core.h"
#include "events.h"
// #include "font.h"
#include "asserts.h"
#include "memorycontrol.h"


ApplicationModeIntro::ApplicationModeIntro() :
	mMusicId          (0),
	m_pSprite         (nullptr) {}

// *************************************************

ApplicationModeIntro::~ApplicationModeIntro()
{
	if (m_pSprite) g_pWindowManager->ReleaseSprite(m_pSprite);
}

// *************************************************

IdMode ApplicationModeIntro::GetId()
{
	return AM_Menu;
}

// *************************************************

void ApplicationModeIntro::Activate()
{
	GAME_ASSERT(g_pEventManager);

	/*m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);*/

	g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);
	g_pEventManager->Register(this, IEventManager::EM_Event::Quit);

	g_pWindowManager->Init();
	//m_pSprite = g_pWindowManager->RequireSprite(Vec2(SCR_HEIGHT/4.f, 400.f), Vec2(400.f, 56.f), (DATA_FOLDER + "title.png").c_str(), false);

	//mMusicId = g_pSoundManager->LoadWav((DATA_FOLDER + "Superboy.wav").c_str());
}

// *************************************************

void ApplicationModeIntro::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	g_pSoundManager->UnloadWav(mMusicId);
	g_pWindowManager->End();
}

// *************************************************

void ApplicationModeIntro::ProcessInput()
{
	GAME_ASSERT(g_pEventManager);
	g_pEventManager->UpdateEvents();
}

// *************************************************

void ApplicationModeIntro::Run(float deltaTime)
{
}

// *************************************************

void ApplicationModeIntro::Render()
{
	g_pWindowManager->Render();
	//glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	//glClear( GL_COLOR_BUFFER_BIT );

	//glColor3f(1.f, 1.f, 1.f);
	//if (m_pSprite) g_pGraphicsEngine->RenderSprite(m_pSprite);
	//mCurrentContainer->Render();
}

// *************************************************

bool ApplicationModeIntro::ProcessEvent(const Event& event)
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

void ApplicationModeIntro::StartLevel(Game::GameLevel level)
{
	g_gameLevel = level;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************

void ApplicationModeIntro::QuitGame()
{
	gQuit = true;
}