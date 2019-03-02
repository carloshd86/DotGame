#include "globals.h"
#include "applicationmodeintro.h"
#include "applicationmanager.h"
#include "core.h"
#include "events.h"
#include "fontmanager.h"
#include "asserts.h"
#include "memorycontrol.h"

const float ApplicationModeIntro::TIME_TO_START_GAME = 5.f;

// *************************************************

ApplicationModeIntro::ApplicationModeIntro() :
	mMusicId     (0),
	m_pSprite    (nullptr),
    mTimeElapsed (0.f),
	mRestingTime (static_cast<int>(TIME_TO_START_GAME)) {}

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
	GAME_ASSERT(g_pFontManager);

	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);

	g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);
	g_pEventManager->Register(this, IEventManager::EM_Event::Quit);

	g_pWindowManager->Init();

	mTimeElapsed = 0.f;
	mRestingTime = static_cast<int>(TIME_TO_START_GAME);

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
	//g_pSoundManager->UnloadWav(mMusicId);
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
	mTimeElapsed += deltaTime;
	if (mTimeElapsed >= TIME_TO_START_GAME) StartLevel(Game::GameLevel::Level1);
	else if (static_cast<int>(TIME_TO_START_GAME - mTimeElapsed) < mRestingTime)
	{
		mRestingTime = static_cast<int>(TIME_TO_START_GAME - mTimeElapsed);
	}
}

// *************************************************

void ApplicationModeIntro::Render()
{
	g_pFontManager->RenderClear();

	g_pWindowManager->Render();
	g_pFontManager->DrawText(Vec2(100, 100), m_pProperties->GetProperty("intro.title.text").c_str());
	g_pFontManager->DrawText(Vec2(100, 200), m_pProperties->GetProperty("intro.click_start.text").c_str());
	std::string timeMessage = m_pProperties->GetProperty("intro.time_start.text");
	timeMessage.append(std::to_string(mRestingTime));
	g_pFontManager->DrawText(Vec2(100, 300), timeMessage.c_str());
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