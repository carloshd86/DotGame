#include "componentdot.h"
#include "events.h"
#include "globals.h"
#include "asserts.h"

CDot::CDot(Entity * owner, Game::DotType type, int tile, float timeToChangePhase) :
	Component                 (owner),
	mType                     (type),
	mTile                     (tile),
	mTimeToChangePhase        (timeToChangePhase),
	mCurrentTimeToChangePhase (0.f) {}

// *************************************************

CDot::~CDot()
{
	End();
}

// *************************************************

void CDot::Init()
{
	if (!mInitialized)
	{
		GAME_ASSERT(g_pEventManager);
		g_pEventManager->Register(this, IEventManager::EM_Event::MouseClick);

		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireDotType, std::bind(&CDot::OnRequireDotType, this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireTile,    std::bind(&CDot::OnRequireTile,    this, std::placeholders::_1)));

		mInitialized = true;
	}

}

// *************************************************

void CDot::End()
{
	if (mInitialized)
	{
		GAME_ASSERT(g_pEventManager);
		g_pEventManager->Unregister(this);

		mInitialized = false;
	}
}

// *************************************************

void CDot::Run(float deltaTime)
{
	mCurrentTimeToChangePhase += deltaTime;
	if (mCurrentTimeToChangePhase >= mTimeToChangePhase)
	{
		ChangePhase();
		mCurrentTimeToChangePhase = 0.f;
	}
}

// *************************************************

Game::DotType CDot::GetType() const
{
	return mType;
}

// *************************************************

void CDot::SetType(Game::DotType type)
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

// *************************************************

bool CDot::ProcessEvent(const Event& event) 
{

	switch (event.GetType()) 
	{
	case IEventManager::EM_Event::MouseClick: CheckInteractAction(event);    break;
	}

	return true;
}

// *************************************************

void CDot::ReceiveMessage(GameMessage &message)
{
	GameMessage::GM_Type messageType = message.GetType();
	if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
		mMessageCallbacks[messageType](message);
}

// *************************************************

void CDot::CheckInteractAction(const Event& event)
{
	const EventMouseClick* eMouseClick = dynamic_cast<const EventMouseClick*>(&event);
	if (eMouseClick)
	{
		if (IsPositionOverDot(Vec2(gMouseX, gMouseY)))
		{
			if (EventMouseClick::EMouseButton::Left == eMouseClick->GetMouseButton())
			{
				if (Game::DotType::Green == mType) { g_pSoundManager->PlaySound(gHitSoundId);  g_pGame->IncreaseScore(mOwner);			     }
				else                               { g_pSoundManager->PlaySound(gFailSoundId); g_pGame->SetGameOver(Game::GameResult::Fail); }
			}
			else if (EventMouseClick::EMouseButton::Right == eMouseClick->GetMouseButton())
			{
				if (Game::DotType::Red == mType) { g_pSoundManager->PlaySound(gHitSoundId);  g_pGame->IncreaseScore(mOwner);			   }
				else                             { g_pSoundManager->PlaySound(gFailSoundId); g_pGame->SetGameOver(Game::GameResult::Fail); }
			}
		}
	}
}

// *************************************************

bool CDot::IsPositionOverDot(Vec2 pos) const
{
	bool result = false;
	Vec2 dotPos = mOwner->GetPos();
	RequireRenderSizeMessage rrsm;
	mOwner->ReceiveMessage(rrsm);
	RequireRenderVisibilityMessage rrvm;
	mOwner->ReceiveMessage(rrvm);
	GAME_ASSERT(rrsm.GetProcessed() && rrvm.GetProcessed());
	if (rrvm.GetVisible())
	{
		result = dotPos.x < pos.x && dotPos.x + rrsm.GetX() > pos.x
			     && dotPos.y < pos.y && dotPos.y + rrsm.GetY() > pos.y;
	}
	return result;
}

// *************************************************

void CDot::ChangePhase()
{
	RequireRenderVisibilityMessage rrvm;
	mOwner->ReceiveMessage(rrvm);
	GAME_ASSERT(rrvm.GetProcessed());
	
	SetVisibleMessage svm(!rrvm.GetVisible());
	Vec2 newPos(-999.f,-999.f);
	if (svm.GetVisible())
	{
		mTile = g_pGame->ObtainNewFreeTile();
		newPos = Game::GetPositionFromTileIndex(mTile);
	}
	else
	{
		g_pGame->ReleaseTile(GetTile());
	}
	mOwner->ReceiveMessage(svm);
	mOwner->SetPos(newPos);
}

// *************************************************

void CDot::OnRequireDotType(GameMessage& message)
{
	RequireDotTypeMessage * rdtm = dynamic_cast<RequireDotTypeMessage *>(&message);
	if (rdtm)
	{
		rdtm->SetDotType(mType);
		rdtm->SetProcessed(true);
	}
}

// *************************************************

void CDot::OnRequireTile(GameMessage& message)
{
	RequireTileMessage * rtm = dynamic_cast<RequireTileMessage *>(&message);
	if (rtm)
	{
		rtm->SetTile(mTile);
		rtm->SetProcessed(true);
	}
}