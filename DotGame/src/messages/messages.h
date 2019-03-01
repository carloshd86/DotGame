#ifndef __MESSAGES_H__
#define __MESSAGES_H__


#include "messagereceiver.h"


class GameMessage
{
public:

	enum GM_Type
	{
		EnemyCollision,
		RigidbodyCollision,
		GoalReached,
		SetPosition,
		AddPosition,
		SetSize,
		RequireRenderPosition,
		RequireRenderSize,
		RequireRigidbodyPosition,
		RequireRigidbodySize,
		RequireMovement,
		RequirePursuingSpeed,
		RequireRoute,
		IncreaseCurrentRoutePoint,
		SetMovement,
		MoveUp,
		MoveDown,
		MoveLeft,
		MoveRight,
		ChangeSprite
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
//
// *************************************************

class EnemyCollisionMessage : public GameMessage
{
public:

	EnemyCollisionMessage() :
		GameMessage(GM_Type::EnemyCollision) {}
};

// *************************************************
//
// *************************************************

class RigidbodyCollisionMessage : public GameMessage
{
public:

	enum RGBM_Type 
	{
		CollisionX,
		CollisionY
	};

	RigidbodyCollisionMessage(RigidbodyCollisionMessage::RGBM_Type collisionType) : 
		GameMessage    (GM_Type::RigidbodyCollision),
		mCollisionType (collisionType) {}

	RigidbodyCollisionMessage::RGBM_Type GetCollisionType() const { return mCollisionType; }

private:

	RigidbodyCollisionMessage::RGBM_Type mCollisionType;
};

// *************************************************
//
// *************************************************

class GoalReachedMessage : public GameMessage
{
public:

	GoalReachedMessage() :
		GameMessage(GM_Type::GoalReached) {}
};

// *************************************************
//
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
//
// *************************************************

class AddPositionMessage : public GameMessage
{
public:

	AddPositionMessage(float x, float y) : 
		GameMessage (GM_Type::AddPosition),
		mX          (x),
		mY          (y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

// *************************************************
//
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
//
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
//
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
//
// *************************************************

class RequireRigidbodyPositionMessage : public GameMessage
{
public:

	RequireRigidbodyPositionMessage() : 
		GameMessage (GM_Type::RequireRigidbodyPosition),
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
//
// *************************************************

class RequireRigidbodySizeMessage : public GameMessage
{
public:

	RequireRigidbodySizeMessage() : 
		GameMessage (GM_Type::RequireRigidbodySize),
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
//
// *************************************************

class RequireMovementMessage : public GameMessage
{
public:

	RequireMovementMessage() : 
		GameMessage (GM_Type::RequireMovement),
		mProcessed  (false),
		mX          (0.f), 
		mY          (0.f),
		mSpeed      (0.f){}

	bool  GetProcessed()                  const { return mProcessed; }
	void  SetProcessed(bool processed)          { mProcessed = processed; }
	float GetX()                          const { return mX; }
	void  SetX(float x)                         { mX = x; }
	float GetY()                          const { return mY; }
	void  SetY(float y)                         { mY = y; }
	float GetSpeed()                      const { return mSpeed; }
	void  SetSpeed(float speed)                 { mSpeed = speed; }

private:

	bool  mProcessed;
	float mX;
	float mY;
	float mSpeed;
};

// *************************************************
//
// *************************************************

class RequirePursuingSpeedMessage : public GameMessage
{
public:

	RequirePursuingSpeedMessage() : 
		GameMessage    (GM_Type::RequirePursuingSpeed),
		mPursuingSpeed (0.f) {}

	bool  GetProcessed()                  const { return mProcessed; }
	void  SetProcessed(bool processed)          { mProcessed = processed; }
	float GetPursuingSpeed()              const { return mPursuingSpeed; }
	void  SetPursuingSpeed(float pursuingSpeed) { mPursuingSpeed = pursuingSpeed; }

private:

	bool  mProcessed;
	float mPursuingSpeed;
};

// *************************************************
//
// *************************************************

class RequireRouteMessage : public GameMessage
{
public:

	RequireRouteMessage() : 
		GameMessage        (GM_Type::RequireRoute),
		m_pRoutePoints     (nullptr),
		mCurrentRoutePoint (0) {}

	const std::vector<Vec2> * GetRoutePoints()                                 const { return m_pRoutePoints; }
	void                      SetRoutePoints(const std::vector<Vec2> * pRoutePoints) { m_pRoutePoints = pRoutePoints; }
	uint8_t                   GetCurrentRoutePoint()                           const { return mCurrentRoutePoint; }
	void                      SetCurrentRoutePoint(uint8_t currentRoutePoint)        { mCurrentRoutePoint = currentRoutePoint; }

private:

	const std::vector<Vec2> *m_pRoutePoints;
	uint8_t                  mCurrentRoutePoint;
};

// *************************************************
//
// *************************************************

class IncreaseCurrentRoutePointMessage : public GameMessage
{
public:

	IncreaseCurrentRoutePointMessage(float currentX, float currentY) : 
		GameMessage (GM_Type::IncreaseCurrentRoutePoint),
		mCurrentX   (currentX),
		mCurrentY   (currentY) {}

	float GetCurrentX() const { return mCurrentX; }
	float GetCurrentY() const { return mCurrentY; }

private:

	float mCurrentX;
	float mCurrentY;
};

// *************************************************
//
// *************************************************

class SetMovementMessage : public GameMessage
{
public:

	SetMovementMessage(float x, float y) : 
		GameMessage (GM_Type::SetMovement),
		mX          (x),
		mY          (y) {}

	float GetX() const { return mX; }
	float GetY() const { return mY; }

private:

	float mX;
	float mY;
};

// *************************************************
//
// *************************************************

class MoveUpMessage : public GameMessage
{
public:

	MoveUpMessage() : 
		GameMessage (GM_Type::MoveUp) {}
};

// *************************************************
//
// *************************************************

class MoveDownMessage : public GameMessage
{
public:

	MoveDownMessage() : 
		GameMessage (GM_Type::MoveDown) {}
};

// *************************************************
//
// *************************************************

class MoveLeftMessage : public GameMessage
{
public:

	MoveLeftMessage() :
		GameMessage(GM_Type::MoveLeft) {}
};

// *************************************************
//
// *************************************************

class MoveRightMessage : public GameMessage
{
public:

	MoveRightMessage() : 
		GameMessage (GM_Type::MoveRight) {}
};

// *************************************************
//
// *************************************************

class ChangeSpriteMessage : public GameMessage
{
public:

	ChangeSpriteMessage(const char * image) : 
		GameMessage (GM_Type::ChangeSprite),
		m_pImage    (image) {}

	const char * GetImage() const { return m_pImage; }

private:

	const char * m_pImage;
};

#endif