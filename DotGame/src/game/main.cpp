#include "globals.h"
#include "core.h"
//#include "font.h"
#include "applicationmanager.h"
#include "sdlwindowmanager.h"
#include "sdlinputmanager.h"
//#include "swalibsoundmanager.h"
#include "asserts.h"
#include "memorycontrol.h"
#include <SDL.h>

ApplicationManager * g_pApplicationManager;
Game               * g_pGame;
Game::GameLevel      g_gameLevel;
IWindowManager     * g_pWindowManager;
ISoundManager      * g_pSoundManager;
IEventManager      * g_pEventManager;
bool                 gQuit   = false;
float                gMouseX = 0.f;
float                gMouseY = 0.f;

int main(int argc, char *argv[])
{
	g_pApplicationManager = ApplicationManager::Instance();
	GAME_ASSERT(g_pApplicationManager);

	g_pWindowManager = SdlWindowManager::Instance();
	GAME_ASSERT(g_pWindowManager);
	GAME_ASSERT(g_pWindowManager->Init());

	g_pEventManager = SdlInputManager::Instance();
	GAME_ASSERT(g_pEventManager);

	/*g_pSoundManager = GAME_NEW(SwalibSoundManager, ());
	g_pSoundManager->InitSound();*/

	/*FONT_Init();*/

	// Set up rendering
	//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); // Sets up clipping
	//glClearColor( 0.0f, 0.1f, 0.3f, 0.0f );
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho( 0.0, SCR_WIDTH, 0.0, SCR_HEIGHT, 0.0, 1.0);
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	//FONT_End();
	//g_pSoundManager->EndSound();

	GAME_DELETE(g_pGame);
	GAME_DELETE(g_pSoundManager);
	GAME_DELETE(g_pWindowManager);
	GAME_DELETE(g_pEventManager);
	GAME_DELETE(g_pApplicationManager);

	return 0;
}
