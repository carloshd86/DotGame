#include "componentrenderable.h"
#include "globals.h"
#include "asserts.h"


CRenderable::CRenderable(Entity * owner, Vec2 size, const char * image) :
		Component    (owner),
		mSize        (size),
		mImage       (image),
		mVisible     (true),
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

		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetPosition      , std::bind(&CRenderable::OnSetSize      , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::SetSize          , std::bind(&CRenderable::OnSetSize      , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireRenderSize, std::bind(&CRenderable::OnRequireSize  , this, std::placeholders::_1)));
		mMessageCallbacks.insert(std::pair<GameMessage::GM_Type, MessageCallbackFun>(GameMessage::GM_Type::RequireSprite    , std::bind(&CRenderable::OnRequireSprite, this, std::placeholders::_1)));

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

		mMessageCallbacks.clear();

		mInitialized = false;
	}
}

// *************************************************

ISprite* CRenderable::GetSprite () const { return m_pSprite; }
void CRenderable::Run   (float deltaTime) {}

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
	return mVisible;
}

// *************************************************

void CRenderable::SetVisible(bool visible)
{
	mVisible = visible;
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