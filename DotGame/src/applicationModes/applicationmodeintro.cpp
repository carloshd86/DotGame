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
    mTimeElapsed (0.f),
	mRestingTime (static_cast<int>(TIME_TO_START_GAME))
{
	mMusicId.pId      = nullptr;
}

// *************************************************

ApplicationModeIntro::~ApplicationModeIntro()
{
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

	g_pWindowManager->InitWindow();
	g_pWindowManager->SetBackgroundColor(0.f, 0.f, 0.f);

	mTimeElapsed = 0.f;
	mRestingTime = static_cast<int>(TIME_TO_START_GAME);

	mTitleText          = m_pProperties->GetProperty("intro.title.text");
	mStartText          = m_pProperties->GetProperty("intro.click_start.text");
	mAutomaticStartText = m_pProperties->GetProperty("intro.time_start.text");
}

// *************************************************

void ApplicationModeIntro::Deactivate()
{
	GAME_ASSERT(g_pEventManager);

	Properties::RemoveInstance();
	m_pProperties = nullptr;

	g_pEventManager->Unregister(this);
	if (g_pSoundManager)
	{
		g_pSoundManager->UnloadMusic();
	}
	g_pWindowManager->EndWindow();
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
	else if (static_cast<int>(TIME_TO_START_GAME - mTimeElapsed) < mRestingTime - 1)
	{
		mRestingTime = static_cast<int>(TIME_TO_START_GAME - mTimeElapsed) + 1;
	}
}

// *************************************************

void ApplicationModeIntro::Render()
{
	g_pWindowManager->ClearColorBuffer(0.f, 0.f, 0.f);
	g_pWindowManager->Render();

	g_pFontManager->DrawText(Vec2(100, 100), mTitleText.c_str());
	g_pFontManager->DrawText(Vec2(100, 200), mStartText.c_str());
	std::string timeMessage = mAutomaticStartText;
	timeMessage.append(std::to_string(mRestingTime));
	g_pFontManager->DrawText(Vec2(100, 300), timeMessage.c_str());

	g_pWindowManager->RefreshRendering();
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
	if (g_pSoundManager)
	{
		g_pSoundManager->PlaySound(gStartSoundId);
	}
	g_gameLevel = level;
	g_pApplicationManager->SwitchMode(AM_Game);
}

// *************************************************

void ApplicationModeIntro::QuitGame()
{
	gQuit = true;
}