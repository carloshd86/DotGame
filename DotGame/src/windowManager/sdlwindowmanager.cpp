#include "globals.h"
#include "sdlwindowmanager.h"
#include "core.h"
#include "sdlsprite.h"
#include "memorycontrol.h"
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>

#define mSdlWindow  static_cast<SDL_Window *>(mWindow.pWindow)
#define mSdlContext static_cast<SDL_GLContext *>(mContext.pContext)


SdlWindowManager * SdlWindowManager::mInstance = nullptr;

// *************************************************

SdlWindowManager::SdlWindowManager() : 
	mWindowSurface   (nullptr),
	mBackgroundImage (""),
	mBackground      (nullptr),
	mBackgroundR     (0.f),
	mBackgroundG     (0.f),
	mBackgroundB     (0.f),
	mInitialized     (false), 
	mEnded           (false) 
{
	mWindow.pWindow   = nullptr;
	mContext.pContext = nullptr;
}

// *************************************************

SdlWindowManager::~SdlWindowManager()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************

SdlWindowManager * SdlWindowManager::Instance() {
	if (!mInstance) {
		mInstance = GAME_NEW(SdlWindowManager, ());
	}

	return mInstance;
}

// *************************************************

IWindowManager::GE_Err SdlWindowManager::Init()
{
	if (!mInitialized)
	{
		if (!mBackgroundImage.empty()) mBackground = IMG_Load(mBackgroundImage.c_str());

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			std::cout << "could not initialize SDL" << std::endl;
			SDL_Quit();
			return GE_Err::KO;
		}

		/* Request opengl 3.2 context.
		* SDL doesn't have the ability to choose which profile at this time of writing,
		* but it should default to the core profile */
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		/* Turn on double buffering with a 24bit Z buffer.
		* You may need to change this to 16 or 32 for your system */
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		/* Create our window centered at 512x512 resolution */
		mWindow.pWindow = SDL_CreateWindow("Dot Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!mSdlWindow) {
			std::cout << "Unable to create window" << std::endl;
			SDL_Quit();
			return GE_Err::KO;
		}

		mWindowSurface = SDL_GetWindowSurface(mSdlWindow);
		SDL_FillRect(mWindowSurface, NULL, SDL_MapRGB(mWindowSurface->format, mBackgroundR, mBackgroundG, mBackgroundB));
		SDL_UpdateWindowSurface(mSdlWindow);

		/* Create our opengl context and attach it to our window */
		mContext.pContext = SDL_GL_CreateContext(mSdlWindow);

		/* This makes our buffer swap syncronized with the monitor's vertical refresh */
		SDL_GL_SetSwapInterval(1);

		SDL_AddEventWatch(SdlWindowManager::EventFilter, mSdlWindow);

		mInitialized = true;
	}

	return GE_Err::OK;
}

// *************************************************

IWindowManager::GE_Err SdlWindowManager::End()
{
	if (mInitialized)
	{
		for(auto sprite : mSprites) GAME_DELETE(sprite.first);
		mSprites.clear();

		for (auto& texture : mTextures) SDL_FreeSurface(texture.second);
		mTextures.clear();

		/* Delete our opengl context, destroy our window, and shutdown SDL */
		SDL_GL_DeleteContext(mSdlContext);
		SDL_DestroyWindow(mSdlWindow);
		SDL_Quit();

		mWindowSurface    = nullptr;
		mWindow.pWindow   = nullptr;
		mContext.pContext = nullptr;

		mInitialized = false;
	}
	mEnded = true;
	return GE_Err::OK;
}

// *************************************************

void SdlWindowManager::Render()
{
	glColor3f(mBackgroundR, mBackgroundG, mBackgroundB);
	glClear( GL_COLOR_BUFFER_BIT );

	// Render background
	if (!mBackgroundImage.empty())
	{
		for (int i = 0; i <= SCR_WIDTH / GAME_BACKGROUND_WIDTH; i++)
			for (int j = 0; j <= SCR_HEIGHT / GAME_BACKGROUND_HEIGHT; j++)
				RenderTexture(Vec2(i * GAME_BACKGROUND_WIDTH + 64.f, j * GAME_BACKGROUND_HEIGHT + 64.f), Vec2(GAME_BACKGROUND_WIDTH, GAME_BACKGROUND_HEIGHT), mBackground);
	}

	// Render elements
	for(auto spritePair : mSprites)
	{
		if (spritePair.second)
		{
			ISprite * sprite = spritePair.first;
			glColor3f(sprite->GetRed(), sprite->GetGreen(), sprite->GetBlue());
			RenderSprite(sprite);
		}
	}
}

// *************************************************

void SdlWindowManager::SwapBuffers() {
	SDL_GL_SwapWindow(mSdlWindow);
}

// *************************************************

void SdlWindowManager::ClearColorBuffer(float r, float g, float b) {
	glClearColor(r, g, b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

// *************************************************

ISprite * SdlWindowManager::RequireSprite(Vec2 pos, Vec2 size, const char * image, bool manageRender, float r, float g, float b)
{
	SDL_Surface* spriteId = nullptr;
	for (auto& texture : mTextures) 
	{
		if (texture.first == image) 
		{
			spriteId = static_cast<SDL_Surface*>(texture.second);
			break;
		}
	}

	if (!spriteId) 
	{
		spriteId = IMG_Load(image);
		if (spriteId) mTextures.push_back({ image, spriteId });
	}

	ISprite * requiredSprite = GAME_NEW(SdlSprite, (pos, size, spriteId, r, g, b));
	mSprites.push_back(std::pair<ISprite *, bool>(requiredSprite, manageRender));

	return requiredSprite;
}

// *************************************************

void SdlWindowManager::ReleaseSprite(ISprite * sprite) 
{
	for (auto it = mSprites.begin(); it != mSprites.end(); ) 
	{
		if ((*it).first == sprite) 
		{
			GAME_DELETE((*it).first);  
			it = mSprites.erase(it);
		} 
		else 
		{
			++it;
		}
	}
}

// *************************************************

void SdlWindowManager::RenderSprite(ISprite * sprite) 
{
	RenderTexture(sprite->GetPos(), sprite->GetSize(), static_cast<SDL_Surface*>(sprite->GetTex().pId));
}

// *************************************************

void SdlWindowManager::RenderTexture(Vec2 pos, Vec2 size, SDL_Surface* tex) 
{
	if (tex) {
		SDL_Rect rcDest = { static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y) };
		SDL_BlitSurface(tex, NULL, mWindowSurface, &rcDest);
		SDL_UpdateWindowSurface(mSdlWindow);
	}
}

// *************************************************

void SdlWindowManager::SetBackgroundImage(const char * backgroundImage) 
{
	if(!mBackgroundImage.empty()) SDL_FreeSurface(mBackground);
	mBackgroundImage = backgroundImage;
	mBackground = IMG_Load(mBackgroundImage.c_str());
}

// *************************************************

void SdlWindowManager::SetBackgroundColor(float r, float g, float b) 
{
	mBackgroundR = r;
	mBackgroundG = g;
	mBackgroundB = b;
}

// *************************************************

Vec2 SdlWindowManager::GetWorldSize() 
{
	return Vec2(SCR_WIDTH, SCR_HEIGHT);
}

// *************************************************

bool SdlWindowManager::WindowShouldClose() 
{
	SDL_Event e;
	return (SDL_PollEvent(&e) && e.type == SDL_QUIT);
}

// *************************************************

void SdlWindowManager::PumpEvents() {
	SDL_PumpEvents();
}

// *************************************************

int SdlWindowManager::EventFilter(void* userdata, SDL_Event* event) {

	if (mInstance) {
		if (mInstance->mInputCallbacks.find(static_cast<SDL_EventType>(event->type)) != mInstance->mInputCallbacks.end()) {
			auto& callbackVector = mInstance->mInputCallbacks[static_cast<SDL_EventType>(event->type)];

			// TODO Por cada tipo de evento, llamar con los parámettros que corresponda.
			switch (event->type) {
			case SDL_WINDOWEVENT: {
				SDL_WindowEvent e = event->window;
				switch (e.event) {
				case SDL_WINDOWEVENT_ENTER:
					std::cout << "window enter\n";
					break;
				case SDL_WINDOWEVENT_LEAVE:
					std::cout << "window leave\n";
					break;
				default:
					break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				SDL_MouseMotionEvent e = event->motion;
				std::cout << e.x << ", " << e.y << std::endl;
				if (e.x < 20) {
					std::cout << "relative mode ON\n";
					SDL_SetRelativeMouseMode(SDL_TRUE);
					SDL_PumpEvents();
					SDL_FlushEvent(SDL_WINDOWEVENT_LEAVE);
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN: {
				SDL_MouseButtonEvent e = event->button;
				if (e.button == SDL_BUTTON_RIGHT) {
					SDL_SetRelativeMouseMode(SDL_GetRelativeMouseMode() ? SDL_FALSE
						: SDL_TRUE);
					if (!SDL_GetRelativeMouseMode())
						SDL_WarpMouseInWindow(sdl_window, 800 / 2, 600 / 2);
				}
				break;
			}
			default:
				break;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}

// *************************************************

void SdlWindowManager::SetMouseMoveCallback(InputCallbackFun fun) {
	mInputCallbacks[SDL_MOUSEMOTION].push_back(fun);
}

// *************************************************

void SdlWindowManager::SetMouseClickCallback(InputCallbackFun fun) {
	mInputCallbacks[SDL_MOUSEMOTION].push_back(fun);
}

// *************************************************

void SdlWindowManager::SetKeyPressedCallback(InputCallbackFun fun) {
	mInputCallbacks[SDL_KEYUP].push_back(fun);
}