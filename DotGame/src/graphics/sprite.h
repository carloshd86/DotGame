#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "core.h"

class ISprite
{

public:
	class SpriteId {
	public:
		void * pId;
	};

	virtual Vec2         GetPos   ()                                    const = 0; 
	virtual void         SetPos   (float x, float y)                          = 0; 
	virtual Vec2         GetSize  ()                                    const = 0; 
	virtual void         SetSize  (float x, float y)                          = 0; 
	virtual SpriteId     GetTex   ()                                    const = 0; 
	virtual void         SetColor (float red, float green, float blue)        = 0;
	virtual float        GetRed   ()                                    const = 0;
	virtual float        GetGreen ()                                    const = 0;
	virtual float        GetBlue  ()                                    const = 0;
};

#endif