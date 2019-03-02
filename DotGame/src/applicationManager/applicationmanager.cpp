#include "globals.h"
#include "applicationmanager.h"
#include "applicationmodegame.h"
#include "applicationmodeintro.h"
#include "applicationmodegameover.h"
#include "memorycontrol.h"


ApplicationManager * ApplicationManager::mInstance = nullptr;

// *************************************************

ApplicationManager::ApplicationManager() : 
	mDesiredMode    (AM_Menu),
	m_pGameMode     (nullptr),
	mLang           (DEFAULT_LANG),
	mAudioActivated (true) {}

// *************************************************

ApplicationManager::~ApplicationManager()
{
	GAME_DELETE(m_pGameMode);
}

// *************************************************

ApplicationManager * ApplicationManager::Instance() {
	if (!mInstance) {
		mInstance = GAME_NEW(ApplicationManager, ());
	}

	return mInstance;
}

// *************************************************

void ApplicationManager::SwitchMode(IdMode mode)
{
	mDesiredMode = mode;
}

// *************************************************

void ApplicationManager::ManageModeChange()
{
	if (!m_pGameMode)
	{
		m_pGameMode = GAME_NEW(ApplicationModeIntro, ());
		m_pGameMode->Activate();
	}
	else if (m_pGameMode->GetId() != mDesiredMode)
	{
		if (m_pGameMode) m_pGameMode->Deactivate();
		GAME_DELETE(m_pGameMode);

		switch (mDesiredMode)
		{
			case AM_Menu           : m_pGameMode = GAME_NEW(ApplicationModeIntro, ());    break;
			case AM_Game           : m_pGameMode = GAME_NEW(ApplicationModeGame, ());     break;
			case AM_GameOver       : m_pGameMode = GAME_NEW(ApplicationModeGameOver, ()); break;
			default                : m_pGameMode = nullptr;                               break; 
		}

		m_pGameMode->Activate();
	}
}

// *************************************************

void ApplicationManager::ProcessInput()
{
	m_pGameMode->ProcessInput();
}

// *************************************************

void ApplicationManager::Run(float deltaTime)
{
	m_pGameMode->Run(deltaTime);
}

// *************************************************

void ApplicationManager::Render()
{
	m_pGameMode->Render();
}

// *************************************************

Properties::P_Language ApplicationManager::GetLang() const
{
	return mLang;
}

// *************************************************

void ApplicationManager::SetLang(Properties::P_Language lang)
{
	mLang = lang;
}

// *************************************************

bool ApplicationManager::IsAudioActivated() const
{
	return mAudioActivated;
}

// *************************************************

void ApplicationManager::SetAudioActivated(bool audioActivated)
{
	mAudioActivated = audioActivated;
}