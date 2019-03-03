#include "globals.h"
#include "sdlwindowmanager.h"
#include "core.h"
#include "sdlsprite.h"
#include "memorycontrol.h"
#include "asserts.h"
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_video.h>

#define mSdlWindow   static_cast<SDL_Window *>   (mWindow.pWindow)
#define mSdlContext  static_cast<SDL_GLContext *>(mContext.pContext)
#define mSdlRenderer static_cast<SDL_Renderer *> (mRenderer.pRenderer)


SdlWindowManager * SdlWindowManager::mInstance = nullptr;

// *************************************************

SdlWindowManager::SdlWindowManager() : 
	mWindowSurface   (nullptr),
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

		mWindow.pWindow = SDL_CreateWindow("Dot Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (!mSdlWindow) {
			std::cout << "Unable to create window" << std::endl;
			SDL_Quit();
			return GE_Err::KO;
		}

		mWindowSurface = SDL_GetWindowSurface(mSdlWindow);
		SDL_FillRect(mWindowSurface, NULL, SDL_MapRGB(mWindowSurface->format,static_cast<uint8_t>(mBackgroundR * 255), static_cast<uint8_t>(mBackgroundG * 255), static_cast<uint8_t>(mBackgroundB * 255)));
		SDL_UpdateWindowSurface(mSdlWindow);

		mContext.pContext = SDL_GL_CreateContext(mSdlWindow);

		mRenderer.pRenderer = SDL_CreateRenderer(mSdlWindow, -1, 0);

		int imgFlags = IMG_INIT_PNG;
		if( !( IMG_Init( imgFlags ) & imgFlags ) )
		{
			printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		}

		mInitialized = true;
	}

	return GE_Err::OK;
}

// *************************************************

IWindowManager::GE_Err SdlWindowManager::End()
{
	if (mInitialized)
	{
		EndWindow();

		IMG_Quit();
		SDL_DestroyRenderer(mSdlRenderer);
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

IWindowManager::GE_Err SdlWindowManager::InitWindow()
{
	return GE_Err::OK;
}

// *************************************************

IWindowManager::GE_Err SdlWindowManager::EndWindow()
{
	for (auto priorities : mSprites)
	{
		for (auto& sprite : priorities.second) GAME_DELETE(sprite.first);
	}
	mSprites.clear();

	for (auto& texture : mTextures) SDL_DestroyTexture(texture.second);
	mTextures.clear();

	return GE_Err::OK;
}

// *************************************************

void SdlWindowManager::Render()
{
	SDL_SetRenderDrawColor(mSdlRenderer, static_cast<uint8_t>(mBackgroundR * 255), static_cast<uint8_t>(mBackgroundG * 255), static_cast<uint8_t>(mBackgroundB * 255), 255);
	SDL_RenderClear(mSdlRenderer);

	// Render elements
	for (auto priorities : mSprites)
	{
		for (auto& spritePair : priorities.second)
		{
			if (spritePair.second)
			{
				ISprite * sprite = spritePair.first;
				if (sprite->GetVisible())
				{
					SDL_SetRenderDrawColor(mSdlRenderer, static_cast<uint8_t>(sprite->GetRed() * 255), static_cast<uint8_t>(sprite->GetGreen() * 255), static_cast<uint8_t>(sprite->GetBlue() * 255), 255);
					RenderSprite(sprite);
				}
			}
		}
	}
	SDL_RenderPresent(mSdlRenderer);
}

// *************************************************

void SdlWindowManager::SwapBuffers() {
	SDL_GL_SwapWindow(mSdlWindow);
}

// *************************************************

void SdlWindowManager::ClearColorBuffer(float r, float g, float b) {
	SDL_SetRenderDrawColor(mSdlRenderer, static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255), 255);
	SDL_RenderClear(mSdlRenderer);
}

// *************************************************

ISprite * SdlWindowManager::RequireSprite(Vec2 pos, Vec2 size, const char * image, int priority, bool manageRender, float r, float g, float b)
{
	SDL_Texture* spriteId = nullptr;
	for (auto& texture : mTextures) 
	{
		if (texture.first == image) 
		{
			spriteId = static_cast<SDL_Texture*>(texture.second);
			break;
		}
	}

	if (!spriteId) 
	{
		SDL_Surface* surface = IMG_Load(image);
		if (!surface)
		{
			printf( "Couldn't load image! SDL_image Error: %s\n", IMG_GetError() );
		}
		else
		{
			spriteId = SDL_CreateTextureFromSurface(mSdlRenderer, surface);
			if(!spriteId)
			{
				printf( "Unable to create texture from %s! SDL Error: %s\n", image, SDL_GetError());
			}
			else
			{
				mTextures.push_back(std::make_pair(image, spriteId));
			}
			SDL_FreeSurface(surface);
		}
	}

	ISprite * requiredSprite = GAME_NEW(SdlSprite, (pos, size, spriteId, r, g, b));
	mSprites[priority].push_back(std::pair<ISprite *, bool>(requiredSprite, manageRender));

	return requiredSprite;
}

// *************************************************

void SdlWindowManager::ReleaseSprite(ISprite * sprite) 
{
	for (auto mapIt = mSprites.begin(); mapIt != mSprites.end(); mapIt++) 
	{
		for (auto it = (*mapIt).second.begin(); it != (*mapIt).second.end(); )
		{
			if ((*it).first == sprite)
			{
				GAME_DELETE((*it).first);
				it = (*mapIt).second.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

// *************************************************

void SdlWindowManager::RenderSprite(ISprite * sprite) 
{
	RenderTexture(sprite->GetPos(), sprite->GetSize(), static_cast<SDL_Texture*>(sprite->GetTex().pId));
}

// *************************************************

void SdlWindowManager::RenderTexture(Vec2 pos, Vec2 size, SDL_Texture* texture) 
{
	if (texture)
	{
		SDL_Rect rcDest = { static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y) };
		SDL_RenderCopy(mSdlRenderer, texture, nullptr, &rcDest);
	}
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
	return gQuit;
}

// *************************************************

void SdlWindowManager::PumpEvents() {
	SDL_PumpEvents();
}

// *************************************************

IWindowManager::Renderer SdlWindowManager::GetRenderer() const
{
	return mRenderer;
}

// *************************************************

void SdlWindowManager::SetColor(float r, float g, float b, float a)
{
	SDL_SetRenderDrawColor(mSdlRenderer, static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255), static_cast<uint8_t>(a * 255));
}

// *************************************************

void SdlWindowManager::DrawLine(float x1, float y1, float x2, float y2)
{
	SDL_RenderDrawLine(mSdlRenderer, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
	SDL_RenderPresent(mSdlRenderer);
}