#ifndef __SDL_SPRITE_H__
#define __SDL_SPRITE_H__

#include "sprite.h"
#include "core.h"
#include <array>
#include <stdint.h>


struct SDL_Texture;

class SdlSprite : public ISprite
{
public:

	SdlSprite(Vec2 pos, Vec2 size, SDL_Texture* tex, float red, float green, float blue);

	Vec2         GetPos     ()                                    const; 
	void         SetPos     (float x, float y); 
	Vec2         GetSize    ()                                    const; 
	void         SetSize    (float x, float y); 
	SpriteId     GetTex     ()                                    const; 
	void         SetColor   (float red, float green, float blue);
	float        GetRed     ()                                    const;
	float        GetGreen   ()                                    const;
	float        GetBlue    ()                                    const;
	bool         GetVisible ()                                    const; 
	void         SetVisible (bool visible); 

private:

	Vec2                   mPos;
	Vec2                   mSize;
	SDL_Texture*           mTex;
	std::array<float, 3>   mColor;
	bool                   mVisible;
};

#endif