#ifndef __SDL_WINDOW_MANAGER_H__
#define __SDL_WINDOW_MANAGER_H__

#include "core.h"
#include "windowmanager.h"
#include <vector>


class SdlSprite;
struct SDL_Surface;
struct SDL_Texture;

typedef std::map<int, std::vector<std::pair<ISprite *, bool>>> SpritePriorityMap;
typedef std::vector<std::pair<std::string, SDL_Texture*>>      TextureVector;

class SdlWindowManager : public IWindowManager
{
public:
	~SdlWindowManager();

	static SdlWindowManager * Instance();

	GE_Err    Init               ();
	GE_Err    End                ();
	GE_Err    InitWindow         ();
	GE_Err    EndWindow          ();
	void      Render             ();
	void      SwapBuffers        ();
	void      ClearColorBuffer   (float r, float g, float b);
	void      RefreshRendering   ();
	ISprite * RequireSprite      (Vec2 pos, Vec2 size, const char * image, int priority = 1, bool manageRender = true, float r = 1.f, float g = 1.f, float b = 1.f);
	void      ReleaseSprite      (ISprite * sprite);
	void      RenderSprite       (ISprite * sprite);
	void      SetBackgroundColor (float r, float g, float b);
	Vec2      GetWorldSize       ();
	bool      WindowShouldClose  ();
	void      PumpEvents         ();
	Renderer  GetRenderer        () const;
	void      SetColor           (float r, float g, float b, float a);
	void      DrawLine           (float x, float y, float x2, float y2);
	
private:
	SdlWindowManager();
	
	static SdlWindowManager *mInstance;

	const float	 GAME_BACKGROUND_WIDTH  = 128;
	const float	 GAME_BACKGROUND_HEIGHT = 128;

	Window            mWindow;
	Context           mContext;
	Renderer          mRenderer;
	SDL_Surface*      mWindowSurface;
	bool              mInitialized;
	bool              mEnded;
	float             mBackgroundR;
	float             mBackgroundG;
	float             mBackgroundB;
	SpritePriorityMap mSprites;
	TextureVector     mTextures;

	void RenderTexture(Vec2 pos, Vec2 size, SDL_Texture* surface);
};

#endif