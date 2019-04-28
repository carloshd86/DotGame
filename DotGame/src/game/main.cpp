#include "globals.h"
#include "core.h"
#include "applicationmanager.h"
#include "sdlwindowmanager.h"
#include "sdlinputmanager.h"
#include "sdlfontmanager.h"
#include "sdlsoundmanager.h"
#include "asserts.h"
#include "memorycontrol.h"
#include <SDL.h>

ApplicationManager  *  g_pApplicationManager = nullptr;
Game                *  g_pGame               = nullptr;
Game::GameLevel        g_gameLevel;
IWindowManager      *  g_pWindowManager      = nullptr;
ISoundManager       *  g_pSoundManager       = nullptr;
IEventManager       *  g_pEventManager       = nullptr;
IFontManager        *  g_pFontManager        = nullptr;

bool                   gQuit        = false;
float                  gMouseX      = 0.f;
float                  gMouseY      = 0.f;
int                    gFinalScore  = 0;
bool                   gGameSuccess = false;
ISoundManager::SoundId gFailSoundId;
ISoundManager::SoundId gHitSoundId;
ISoundManager::SoundId gStartSoundId;


int main(int argc, char *argv[])
{
	g_pApplicationManager = ApplicationManager::Instance();
	GAME_ASSERT(g_pApplicationManager);

	g_pWindowManager = SdlWindowManager::Instance();
	GAME_ASSERT(g_pWindowManager);
	GAME_ASSERT(g_pWindowManager->Init());

	g_pEventManager = SdlInputManager::Instance();
	GAME_ASSERT(g_pEventManager);

	g_pFontManager = SdlFontManager::Instance();
	GAME_ASSERT(g_pFontManager);
	GAME_ASSERT(g_pFontManager->Init());
	g_pFontManager->LoadFont("Lato-Regular.ttf", 30);

	g_pSoundManager = SdlSoundManager::Instance();
	if (g_pSoundManager && g_pSoundManager->Init())
	{
		gFailSoundId  = g_pSoundManager->LoadSound("fail.wav");
		gHitSoundId   = g_pSoundManager->LoadSound("hit.wav");
		gStartSoundId = g_pSoundManager->LoadSound("start.ogg");
	}

	float deltaTime = 0.f;
	Uint32 lastMilliseconds = 0;
	Uint32 currentMilliseconds = 0;

	while (!g_pWindowManager->WindowShouldClose()) 
	{
		currentMilliseconds = SDL_GetTicks();
		deltaTime = (currentMilliseconds - lastMilliseconds) / 1000.f;
		lastMilliseconds = currentMilliseconds;
		g_pApplicationManager->ManageModeChange();

		//ProcessInput
		g_pApplicationManager->ProcessInput();

		//Run
		g_pApplicationManager->Run(deltaTime);

		// Render
		g_pApplicationManager->Render();

		SDL_Delay(17);
		g_pWindowManager->PumpEvents();
	}

	GAME_DELETE(g_pGame);
	GAME_DELETE(g_pSoundManager);
	GAME_DELETE(g_pFontManager);
	GAME_DELETE(g_pEventManager);
	GAME_DELETE(g_pWindowManager);
	GAME_DELETE(g_pApplicationManager);

	return 0;
}
