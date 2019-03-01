#define SDL_MAIN_HANDLED

#include "globals.h"
#include "core.h"
//#include "font.h"
#include "applicationmanager.h"
#include "sdlwindowmanager.h"
//#include "swalibsoundmanager.h"
//#include "sysinputmanager.h"
#include "memorycontrol.h"
#include <SDL.h>


//-----------------------------------------------------------------------------

ApplicationManager * g_pApplicationManager;
Game               * g_pGame;
Game::GameLevel      g_gameLevel;
IWindowManager     * g_pWindowManager;
ISoundManager      * g_pSoundManager;
IEventManager      * g_pEventManager;

//-----------------------------------------------------------------------------

int main()
{
	SDL_SetMainReady();

	g_pApplicationManager = GAME_NEW(ApplicationManager, ());

	/*g_pWindowManager = GAME_NEW(SwalibGraphicsEngine, ());
	g_pWindowManager->Init();*/

	/*g_pSoundManager = GAME_NEW(SwalibSoundManager, ());
	g_pSoundManager->InitSound();*/

	/*g_pEventManager = SysInputManager::Instance();*/

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

	float deltaTime = 1 / 60.f;

	//while (!SYS_GottaQuit())
	//{
	//	g_pApplicationManager->ManageModeChange();

	//	//ProcessInput
	//	g_pApplicationManager->ProcessInput();

	//	//Run
	//	g_pApplicationManager->Run(deltaTime);

	//	// Render
	//	g_pApplicationManager->Render();

	//	SYS_Show();

	//	// Keep system running
	//	SYS_Pump();
	//	SYS_Sleep(17);
	//}

	//FONT_End();
	//g_pSoundManager->EndSound();

	GAME_DELETE(g_pGame);
	GAME_DELETE(g_pSoundManager);
	GAME_DELETE(g_pWindowManager);
	GAME_DELETE(g_pEventManager);
	GAME_DELETE(g_pApplicationManager);

	return 0;
}
