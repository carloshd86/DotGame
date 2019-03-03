#include "componentdot.h"

CDot::CDot(Entity * owner, DotType type, int tile) :
	Component (owner),
	mType     (type),
	mTile     (tile) {}

// *************************************************

CDot::~CDot()
{
	End();
}

// *************************************************

void CDot::Init()
{
	mInitialized = true;
}

// *************************************************

void CDot::End()
{
	mInitialized = false;
}

// *************************************************

void CDot::Run(float deltaTime)
{
}

// *************************************************

CDot::DotType CDot::GetType() const
{
	return mType;
}

// *************************************************

void CDot::SetType(DotType type)
{
	mType = type;
}

// *************************************************

int CDot::GetTile() const
{
	return mTile;
}

// *************************************************

void CDot::SetTile(int tile)
{
	mTile = tile;
}