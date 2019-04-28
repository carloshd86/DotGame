#include "globals.h"
#include "sdlsprite.h"
#include <SDL_image.h>

SdlSprite::SdlSprite(Vec2 pos, Vec2 size, SDL_Texture* tex, float red, float green, float blue) :
	mPos(pos),
	mSize(size),
	mTex (tex),
	mColor { red, green, blue },
	mVisible (true) {}

// *************************************************

Vec2 SdlSprite::GetPos() const
{
	return mPos;
}

// *************************************************

void SdlSprite::SetPos(float x, float y)
{
	mPos.x = x;
	mPos.y = y;
}

// *************************************************

Vec2 SdlSprite::GetSize() const
{
	return mSize;
}

// *************************************************

void SdlSprite::SetSize(float x, float y)
{
	mSize.x = x;
	mSize.y = y;
}

// *************************************************

ISprite::SpriteId SdlSprite::GetTex() const
{
	SpriteId id;
	id.pId = mTex;
	return id;
}

// *************************************************

void SdlSprite::SetColor(float red, float green, float blue)
{
	mColor[0] = red;
	mColor[1] = green;
	mColor[2] = blue;
}

// *************************************************

float SdlSprite::GetRed() const
{
	return mColor[0];
}

// *************************************************

float SdlSprite::GetGreen() const
{
	return mColor[1];
}

// *************************************************

float SdlSprite::GetBlue() const
{
	return mColor[2];
}

// *************************************************

bool SdlSprite::GetVisible() const
{
	return mVisible;
}

// *************************************************

void SdlSprite::SetVisible(bool visible)
{
	mVisible = visible;
}