#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "core.h"

class IFontManager 
{

public:
	enum FM_Err  {
		KO = 0,
		OK = 1
	};

	virtual FM_Err Init        ()                                       =0;
	virtual FM_Err End         ()                                       =0;
	virtual FM_Err LoadFont    (const char * font, size_t size)         =0;
	virtual void   SetColor    (float r, float g, float b)              =0;
	virtual void   DrawText    (Vec2 pos, Vec2 size, const char * text) =0;
	virtual void   DrawText    (Vec2 pos, const char * text)            =0;
	virtual void   RenderClear ()                                    =0;
};

#endif