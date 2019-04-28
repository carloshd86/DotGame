#ifndef __WINDOW_MANAGER_H__
#define __WINDOW_MANAGER_H__

#include "sprite.h"

class IWindowManager
{

public:
	enum GE_Err  {
		KO = 0,
		OK = 1
	};

	class Window {
	public:
		void * pWindow;
	};

	class Context {
	public:
		void * pContext;
	};

	class Renderer {
	public:
		void * pRenderer;
	};

	virtual GE_Err    Init               ()                                                                                                                                 = 0;
	virtual GE_Err    End                ()                                                                                                                                 = 0;
	virtual GE_Err    InitWindow         ()                                                                                                                                 = 0;
	virtual GE_Err    EndWindow          ()                                                                                                                                 = 0;
	virtual void      Render             ()                                                                                                                                 = 0;
	virtual void      SwapBuffers        ()                                                                                                                                 = 0;
	virtual void      ClearColorBuffer   (float r, float g, float b)                                                                                                        = 0;
	virtual void      RefreshRendering   ()                                                                                                                                 = 0;
	virtual ISprite*  RequireSprite      (Vec2 pos, Vec2 size, const char * image, int priority = 1, bool manageRender = true, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
	virtual void      ReleaseSprite      (ISprite * sprite)                                                                                                                 = 0;
	virtual void      RenderSprite       (ISprite * sprite)                                                                                                                 = 0;
	virtual void      SetBackgroundColor (float r, float g, float b)                                                                                                        = 0;
	virtual Vec2      GetWorldSize       ()                                                                                                                                 = 0;
	virtual bool      WindowShouldClose  ()                                                                                                                                 = 0;
	virtual void      PumpEvents         ()                                                                                                                                 = 0;
	virtual Renderer  GetRenderer        () const                                                                                                                           = 0;
	virtual void      SetColor           (float r, float g, float b, float a)                                                                                               = 0;
	virtual void      DrawLine           (float x, float y, float x2, float y2)                                                                                             = 0;
};

#endif