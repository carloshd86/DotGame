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

	virtual GE_Err    Init               ()                                                                                                               = 0;
	virtual GE_Err    End                ()                                                                                                               = 0;
	virtual void      Render             ()                                                                                                               = 0;
	virtual void      SwapBuffers        ()                                                                                                               = 0;
	virtual void      ClearColorBuffer   (float r, float g, float b)                                                                                      = 0;
	virtual ISprite * RequireSprite      (Vec2 pos, Vec2 size, const char * image, bool manageRender = true, float r = 1.f, float g = 1.f, float b = 1.f) = 0;
	virtual void      ReleaseSprite      (ISprite * sprite)                                                                                               = 0;
	virtual void      RenderSprite       (ISprite * sprite)                                                                                               = 0;
	virtual void      SetBackgroundImage (const char * backgroundImage)                                                                                   = 0;
	virtual void      SetBackgroundColor (float r, float g, float b)                                                                                      = 0;
	virtual Vec2      GetWorldSize       ()                                                                                                               = 0;
	virtual bool      WindowShouldClose  ()                                                                                                               = 0;
	virtual void      PumpEvents         ()                                                                                                               = 0;
};

#endif