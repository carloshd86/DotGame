#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "windowmanager.h"
#include "soundmanager.h"
#include "eventmanager.h"
#include "fontmanager.h"
#include "properties.h"
#include "game.h"


class ApplicationManager;

extern ApplicationManager * g_pApplicationManager;
extern Game               * g_pGame;
extern Game::GameLevel      g_gameLevel;
extern IWindowManager     * g_pWindowManager;
extern ISoundManager      * g_pSoundManager;
extern IEventManager      * g_pEventManager;
extern IFontManager       * g_pFontManager;

extern bool  gQuit;
extern float gMouseX;
extern float gMouseY;
extern int   gFinalScore;
extern bool  gGameSuccess;

extern ISoundManager::SoundId gFailSoundId;
extern ISoundManager::SoundId gHitSoundId;
extern ISoundManager::SoundId gStartSoundId;

const int SCR_WIDTH  = 1280;
const int SCR_HEIGHT = 720;

const float FRAME_WIDTH  = 64;
const float FRAME_HEIGHT = 64;
const int   NUM_COLS     = 10;
const int   NUM_ROWS     = 10;
const int   GRID_POS_X   = 320;
const int   GRID_POS_Y   = 70;
const int   SCORE_POS_X  = 320;
const int   SCORE_POS_Y  = 4;



const std::string DATA_FOLDER = "data/";

const Properties::P_Language DEFAULT_LANG = Properties::P_Language::English;

#endif
