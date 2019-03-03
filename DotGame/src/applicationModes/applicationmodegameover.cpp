#include "globals.h"
#include "applicationmodegameover.h"
#include "applicationmanager.h"
#include "core.h"
#include "events.h"
#include "fontmanager.h"
#include "asserts.h"
#include "memorycontrol.h"

const float ApplicationModeGameOver::TIME_TO_START_GAME = 5.f;

// *************************************************

ApplicationModeGameOver::ApplicationModeGameOver () :
	mMusicId     (0),
	mTimeElapsed (0.f),
	mRestingTime (static_cast<int>(TIME_TO_START_GAME)) {}

// *************************************************

ApplicationModeGameOver::~ApplicationModeGameOver()
{
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
	GAME_ASSERT(g_pFontManager);

	m_pProperties = Properties::Instance("messages", g_pApplicationManager->GetLang());
	GAME_ASSERT(m_pProperties);

	g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);
	g_pEventManager->Register(this, IEventManager::EM_Event::Quit);

	g_pWindowManager->InitWindow();
	g_pWindowManager->SetBackgroundColor(0.f, 0.f, 0.f);

	mTimeElapsed = 0.f;
	mRestingTime = static_cast<int>(TIME_TO_START_GAME);

	if (gGameSuccess) mTitleText = m_pProperties->GetProperty("game_over.win.text");
	else              mTitleText = m_pProperties->GetProperty("game_over.title.text");
	mScoreText = m_pProperties->GetProperty("game_over.final_score.text");
	mScoreText.append(std::to_string(gFinalScore));
	mStartText          = m_pProperties->GetProperty("game_over.click_start.text");
	mAutomaticStartText = m_pProperties->GetProperty("game_over.time_start.text");

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
	g_pWindowManager->EndWindow();
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
		StartLevel(Game::GameLevel::Level1);
	else if (static_cast<int>(TIME_TO_START_GAME - mTimeElapsed) < mRestingTime - 1)
	{
		mRestingTime = static_cast<int>(TIME_TO_START_GAME - mTimeElapsed) + 1;
	}
}

// *************************************************

void ApplicationModeGameOver::Render()
{
	g_pWindowManager->Render();
	g_pFontManager->DrawText(Vec2(100, 100), mTitleText.c_str());
	g_pFontManager->DrawText(Vec2(100, 200), mScoreText.c_str());

	g_pFontManager->DrawText(Vec2(100, 400), mStartText.c_str());
	std::string timeMessage = mAutomaticStartText;
	timeMessage.append(std::to_string(mRestingTime));
	g_pFontManager->DrawText(Vec2(100, 500), timeMessage.c_str());
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