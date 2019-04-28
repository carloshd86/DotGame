#ifndef __SDL_FONT_MANAGER_H__
#define __SDL_FONT_MANAGER_H__

#include "fontmanager.h"
#include <SDL_ttf.h>
#include <set>

class SdlFontManager : public IFontManager
{

public:
	~SdlFontManager();

	static SdlFontManager * Instance();

	FM_Err Init        ();
	FM_Err End         ();
	FM_Err LoadFont    (const char * font, size_t size);
	void   SetColor    (float r, float g, float b);
	void   DrawText    (Vec2 pos, Vec2 size, const char * text);
	void   DrawText    (Vec2 pos, const char * text);

private:
	SdlFontManager();

	static SdlFontManager *mInstance;

	bool          mInitialized;
	bool          mEnded;
	std::string   mFontFile;
	size_t        mFontSize;
	TTF_Font*     mFont;
	SDL_Surface*  mFontSurface;
	SDL_Texture*  mFontTexture;
	SDL_Renderer* mRenderer;
	float         mRed;
	float         mGreen;
	float         mBlue;
};

#endif