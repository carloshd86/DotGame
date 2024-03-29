#include "componentrenderable.h"
#include "globals.h"
#include "asserts.h"


CRenderable::CRenderable(Entity * owner, Vec2 size, const char * image) :
		Component    (owner),
		mSize        (size),
		mImage       (image),
		mInitialized (false) {}

// *************************************************

CRenderable::~CRenderable()
	{
		End();
	}

// *************************************************

void CRenderable::Init()
{
	if (!mInitialized)
	{
		GAME_ASSERT(g_pWindowManager);
		m_pSprite = g_pWindowManager->RequireSprite(mOwner->GetPos(), mSize, mImage.c_str());

		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetPosition            , std::bind(&CRenderable::OnSetPosition   , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetSize                , std::bind(&CRenderable::OnSetSize       , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetVisible             , std::bind(&CRenderable::OnSetVisible    , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRenderSize      , std::bind(&CRenderable::OnRequireSize   , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireSprite          , std::bind(&CRenderable::OnRequireSprite , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRenderVisibility, std::bind(&CRenderable::OnRequireVisible, this, std::placeholders::_1)));

		mInitialized = true;
	}
}

// *************************************************

void CRenderable::End()
{
	if (mInitialized)
	{
		GAME_ASSERT(g_pWindowManager);
		g_pWindowManager->ReleaseSprite(m_pSprite);

		m_pSprite = nullptr;
		mMessageCallbacks.clear();

		mInitialized = false;
	}
}

// *************************************************

ISprite* CRenderable::GetSprite() const 
{ 
	return m_pSprite; 
}

// *************************************************

void CRenderable::Run(float deltaTime)
{
}

// *************************************************

Vec2 CRenderable::GetSize() const
{
	return mSize;
}

// *************************************************

void CRenderable::SetSize(float x, float y)
{
	mSize.x = x;
	mSize.y = y;
	m_pSprite->SetSize(x, y);
}

// *************************************************

bool CRenderable::GetVisible() const
{
	return m_pSprite->GetVisible();
}

// *************************************************

void CRenderable::SetVisible(bool visible)
{
	m_pSprite->SetVisible(visible);
}

// *************************************************

void CRenderable::ReceiveMessage(GameMessage &message)
{
	GameMessage::GM_Type messageType = message.GetType();
	if (mMessageCallbacks.end() != mMessageCallbacks.find(messageType))
		mMessageCallbacks[messageType](message);
}

// *************************************************

void CRenderable::OnSetSize(GameMessage& message)
{
	SetSizeMessage * ssm = dynamic_cast<SetSizeMessage *>(&message);
	if (ssm)
	{
		SetSize(ssm->GetX(), ssm->GetY());
	}
}

// *************************************************

void CRenderable::OnSetPosition(GameMessage& message)
{
	SetPositionMessage * spm = dynamic_cast<SetPositionMessage *>(&message);
	if (spm)
	{
		m_pSprite->SetPos(spm->GetX(), spm->GetY());
	}
}

// *************************************************

void CRenderable::OnSetVisible(GameMessage& message)
{
	SetVisibleMessage * svm = dynamic_cast<SetVisibleMessage *>(&message);
	if (svm)
	{
		m_pSprite->SetVisible(svm->GetVisible());
	}
}

// *************************************************

void CRenderable::OnRequireSize(GameMessage& message)
{
	RequireRenderSizeMessage * rsm = dynamic_cast<RequireRenderSizeMessage *>(&message);
	if (rsm)
	{
		rsm->SetX(mSize.x);
		rsm->SetY(mSize.y);
		rsm->SetProcessed(true);
	}
}

// *************************************************

void CRenderable::OnRequireSprite(GameMessage& message)
{
	RequireSpriteMessage * rsm = dynamic_cast<RequireSpriteMessage *>(&message);
	if (rsm)
	{
		rsm->SetSprite(m_pSprite);
		rsm->SetProcessed(true);
	}
}

// *************************************************

void CRenderable::OnRequireVisible(GameMessage& message)
{
	RequireRenderVisibilityMessage * rrvm = dynamic_cast<RequireRenderVisibilityMessage *>(&message);
	if (rrvm)
	{
		if (m_pSprite) rrvm->SetVisible(m_pSprite->GetVisible());
		else           rrvm->SetVisible(false);
		rrvm->SetProcessed(true);
	}
}