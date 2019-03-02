#ifndef __SDL_WINDOW_MANAGER_H__
#define __SDL_WINDOW_MANAGER_H__

#include "core.h"
#include "windowmanager.h"
#include <vector>


class SdlSprite;
struct SDL_Surface;

class SdlWindowManager : public IWindowManager
{
public:
	~SdlWindowManager();

	static SdlWindowManager * Instance();

	GE_Err    Init               ();
	GE_Err    End                ();
	void      Render             ();
	void      SwapBuffers        ();
	void      ClearColorBuffer   (float r, float g, float b);
	ISprite * RequireSprite      (Vec2 pos, Vec2 size, const char * image, bool manageRender = true, float r = 1.f, float g = 1.f, float b = 1.f);
	void      ReleaseSprite      (ISprite * sprite);
	void      RenderSprite       (ISprite * sprite);
	void      SetBackgroundImage (const char * backgroundImage);
	void      SetBackgroundColor (float r, float g, float b);
	Vec2      GetWorldSize       ();
	bool      WindowShouldClose  ();
	void      PumpEvents         ();
	
private:
	SdlWindowManager();
	
	static SdlWindowManager *mInstance;

	const float	 GAME_BACKGROUND_WIDTH  = 128;
	const float	 GAME_BACKGROUND_HEIGHT = 128;

	Window                                                 mWindow;
	Context                                                mContext;
	SDL_Surface*                                           mWindowSurface;
	bool                                                   mInitialized;
	bool                                                   mEnded;
	std::string                                            mBackgroundImage;
	SDL_Surface*                                           mBackground;
	float                                                  mBackgroundR;
	float                                                  mBackgroundG;
	float                                                  mBackgroundB;
	std::vector<std::pair<ISprite *, bool>>                mSprites;
	std::vector<std::pair<std::string, SDL_Surface*>>      mTextures;

	void RenderTexture(Vec2 pos, Vec2 size, SDL_Surface* tex);
};

#endif