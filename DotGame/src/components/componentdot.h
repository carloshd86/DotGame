#ifndef __COMPONENT_DOT_H__
#define __COMPONENT_DOT_H__

#include "component.h"
#include "eventmanager.h"

class CDot : public Component, public IEventManager::IListener, public IMessageReceiver
{
public:
	CDot(Entity* owner, Game::DotType type, int tile);

	virtual ~CDot();

	void Init();
	void End();

	void Run(float deltaTime);

	Game::DotType GetType() const;
	void          SetType(Game::DotType type);
	int           GetTile() const;
	void          SetTile(int tile);

	void CheckInteractAction(const Event& event);
	bool IsPositionOverDot(Vec2 pos) const;

	bool ProcessEvent(const Event& event);
	void ReceiveMessage(GameMessage &message);

private:
	bool               mInitialized;
	Game::DotType      mType;
	int                mTile;
	MessageCallbackMap mMessageCallbacks;

	void OnRequireDotType(GameMessage& message);
	void OnRequireTile   (GameMessage& message);
};


#endif
