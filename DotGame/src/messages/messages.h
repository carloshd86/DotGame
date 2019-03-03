#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include "messagereceiver.h"
#include "game.h"
#include "sprite.h"

class GameMessage
{
public:
	enum GM_Type
	{
		SetPosition,
		SetSize,
		SetVisible,
		RequireRenderPosition,
		RequireRenderSize,
		RequireRenderVisibility,
		SetTile,
		RequireTile,
		RequireDotType,
		RequireSprite
	};

	GameMessage(GM_Type type) :
		mType(type) {}
	virtual ~GameMessage() {};

	virtual GM_Type GetType() const
	{
		return mType;
	}

private:
	GM_Type mType;
};

// *************************************************

class SetPositionMessage : public GameMessage
{
public:
	SetPositionMessage(float x, float y) : 
		GameMessage (GM_Type::SetPosition),
		mX          (x),
		mY          (y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:
	float mX;
	float mY;
};

// *************************************************

class SetSizeMessage : public GameMessage
{
public:
	SetSizeMessage(float x, float y) : 
		GameMessage (GM_Type::SetSize),
		mX          (x),
		mY          (y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:
	float mX;
	float mY;
};

// *************************************************

class SetVisibleMessage : public GameMessage
{
public:
	SetVisibleMessage(bool visible) : 
		GameMessage (GM_Type::SetVisible),
		mVisible    (visible) {}

	bool GetVisible() const { return mVisible; }

private:
	bool mVisible;
};

// *************************************************

class RequireRenderPositionMessage : public GameMessage
{
public:
	RequireRenderPositionMessage() : 
		GameMessage (GM_Type::RequireRenderPosition),
		mProcessed  (false),
		mX          (0.f), 
		mY          (0.f) {}

	bool  GetProcessed()         const { return mProcessed; }
	void  SetProcessed(bool processed) { mProcessed = processed; }
	float GetX()                 const { return mX; }
	void  SetX(float x)                { mX = x; }
	float GetY()                 const { return mY; }
	void  SetY(float y)                { mY = y; }

private:
	bool  mProcessed;
	float mX;
	float mY;
};

// *************************************************

class RequireRenderSizeMessage : public GameMessage
{
public:
	RequireRenderSizeMessage() : 
		GameMessage (GM_Type::RequireRenderSize),
		mProcessed  (false),
		mX          (0.f), 
		mY          (0.f) {}

	bool  GetProcessed()         const { return mProcessed; }
	void  SetProcessed(bool processed) { mProcessed = processed; }
	float GetX()                 const { return mX; }
	void  SetX(float x)                { mX = x; }
	float GetY()                 const { return mY; }
	void  SetY(float y)                { mY = y; }

private:
	bool  mProcessed;
	float mX;
	float mY;
};

// *************************************************

class RequireRenderVisibilityMessage : public GameMessage
{
public:
	RequireRenderVisibilityMessage() : 
		GameMessage (GM_Type::RequireRenderVisibility),
		mProcessed  (false),
		mVisible    (true) {}

	bool  GetProcessed()         const { return mProcessed; }
	void  SetProcessed(bool processed) { mProcessed = processed; }
	bool  GetVisible()           const { return mVisible; }
	void  SetVisible(bool visible)     { mVisible = visible; }

private:
	bool mProcessed;
	bool mVisible;
};

// *************************************************

class SetTileMessage : public GameMessage
{
public:
	SetTileMessage(int tile) : 
		GameMessage (GM_Type::SetTile),
		mTile       (tile) {}

	int GetTile() const { return mTile; }

private:
	int mTile;
};

// *************************************************

class RequireTileMessage : public GameMessage
{
public:
	RequireTileMessage() : 
		GameMessage (GM_Type::RequireTile),
		mProcessed  (false),
		mTile       (-1) {}

	bool GetProcessed()         const { return mProcessed; }
	void SetProcessed(bool processed) { mProcessed = processed; }
	int  GetTile()              const { return mTile; }
	void SetTile(int tile)            { mTile = tile; }

private:
	bool mProcessed;
	int  mTile;
};

// *************************************************

class RequireDotTypeMessage : public GameMessage
{
public:
	RequireDotTypeMessage() : 
		GameMessage (GM_Type::RequireDotType),
		mProcessed  (false),
		mDotType    (Game::DotType::Green) {}

	bool          GetProcessed()              const { return mProcessed; }
	void          SetProcessed(bool processed)      { mProcessed = processed; }
	Game::DotType GetDotType  ()              const { return mDotType; }
	void          SetDotType(Game::DotType dotType) { mDotType = dotType; }

private:
	bool          mProcessed;
	Game::DotType mDotType;
};

// *************************************************

class RequireSpriteMessage : public GameMessage
{
public:
	RequireSpriteMessage() : 
		GameMessage (GM_Type::RequireSprite),
		mProcessed  (false),
		m_pSprite   (nullptr) {}

	bool     GetProcessed()          const { return mProcessed; }
	void     SetProcessed(bool processed)  { mProcessed = processed; }
	ISprite* GetSprite   ()          const { return m_pSprite; }
	void     SetSprite   (ISprite* sprite) { m_pSprite = sprite; }

private:
	bool     mProcessed;
	ISprite* m_pSprite;
};

#endif