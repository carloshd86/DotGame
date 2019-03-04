#include "sdlfontmanager.h"
#include "globals.h"
#include "memorycontrol.h"
#include "asserts.h"

SdlFontManager * SdlFontManager::mInstance = nullptr;

// *************************************************

SdlFontManager::SdlFontManager() :
	mInitialized (false),
	mEnded       (false),
	mFont        (nullptr),
	mFontSurface (nullptr),
	mFontTexture (nullptr),
	mFontSize    (-1),
	mRed         (1.f),
	mGreen       (1.f),
	mBlue        (1.f),
    mRenderer    (nullptr) {}

// *************************************************

SdlFontManager::~SdlFontManager()
{
	if (mInitialized && !mEnded) End();
}

// *************************************************

SdlFontManager * SdlFontManager::Instance()
{
	if (!mInstance) {
		mInstance = GAME_NEW(SdlFontManager, ());
	}

	return mInstance;
}

// *************************************************

IFontManager::FM_Err SdlFontManager::Init()
{
	if (!mInitialized)
	{
		GAME_ASSERT(g_pWindowManager);
		if (TTF_Init() == -1)
		{
			return KO;
		}

		mRenderer = static_cast<SDL_Renderer*>(g_pWindowManager->GetRenderer().pRenderer);

	}
	return OK;
}

// *************************************************

IFontManager::FM_Err SdlFontManager::End()
{
	if (!mEnded)
	{
		if (mFontTexture)
		{
			SDL_DestroyTexture(mFontTexture);
			mFontTexture = nullptr;
		}
		if (mFontSurface)
		{
			SDL_FreeSurface(mFontSurface);
			mFontSurface = nullptr;
		}
		if (mFont)
		{ 
			TTF_CloseFont(mFont);
			mFont = nullptr;
		}
		TTF_Quit();
	}
	return OK;
}

// *************************************************

IFontManager::FM_Err SdlFontManager::LoadFont(const char * font, size_t size) 
{
	if (mFontFile != font || mFontSize != size)
	{
		if (mFont)
		{
			TTF_CloseFont(mFont);
			mFont = nullptr;
		}
		mFont = TTF_OpenFont((DATA_FOLDER + font).c_str(), size);
		if (!mFont)
		{
			printf("Unable to load font! SDL_ttf Error: %s\n", TTF_GetError());
		}
	}
	return OK;
}

void SdlFontManager::SetColor(float r, float g, float b)
{
	mRed   = r;
	mGreen = g;
	mBlue  = b;
}

// *************************************************

void SdlFontManager::DrawText(Vec2 pos, const char * text)
{
	DrawText(pos, Vec2(0,0), text);
}

// *************************************************

void SdlFontManager::DrawText(Vec2 pos, Vec2 size, const char * text)
{
	if (mFont)
	{
		if (mFontSurface)
		{
			SDL_FreeSurface(mFontSurface);
			mFontSurface = nullptr;
		}

		SDL_Color textColor = { static_cast<uint8_t>(mRed * 255), static_cast<uint8_t>(mGreen * 255), static_cast<uint8_t>(mBlue * 255) };
		mFontSurface = TTF_RenderText_Solid(mFont, text, textColor);
		if(!mFontSurface)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			if (mFontTexture)
			{
				SDL_DestroyTexture(mFontTexture);
				mFontTexture = nullptr;
			}

			mFontTexture = SDL_CreateTextureFromSurface(mRenderer, mFontSurface);
			if(!mFontTexture)
			{
				printf("Unable to create text texture! SDL_ttf Error: %s\n", TTF_GetError());
			}
			else
			{
				SDL_Rect rcDest  = { static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(size.x), static_cast<int>(size.y) };
				if (size.squareLength() < 1)
				{
					SDL_QueryTexture(mFontTexture, NULL, NULL, &rcDest.w, &rcDest.h);
				}
				SDL_RenderCopy(mRenderer, mFontTexture, nullptr, &rcDest);
				//SDL_RenderPresent(mRenderer);

				SDL_DestroyTexture(mFontTexture);
				mFontTexture = nullptr;
			}
			SDL_FreeSurface(mFontSurface);
			mFontSurface = nullptr;
		}
	}
}