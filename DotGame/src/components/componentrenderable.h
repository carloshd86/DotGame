#ifndef __COMPONENT_RENDERABLE_H__
#define __COMPONENT_RENDERABLE_H__

#include "component.h"
#include "sprite.h"

class CRenderable : public Component, public IMessageReceiver
{
public:

	CRenderable(Entity * owner, Vec2 size, const char * image);

	virtual ~CRenderable();

	void Init();
	void End();

	ISprite* GetSprite () const;
	void     Run       (float deltaTime);

	Vec2 GetSize() const;
	void SetSize(float x, float y);

	bool GetVisible() const;
	void SetVisible(bool visible);

	void ReceiveMessage(GameMessage &message);

private:

	Vec2               mSize;
	std::string        mImage;
	ISprite           *m_pSprite;
	bool               mVisible;
	bool               mInitialized;
	MessageCallbackMap mMessageCallbacks;

	void OnSetVisible    (GameMessage& message);
	void OnSetPosition   (GameMessage& message);
	void OnSetSize       (GameMessage& message);
	void OnRequireSize   (GameMessage& message);
	void OnRequireSprite (GameMessage& message);
};


#endif
