#ifndef __COMPONENT_H__
#define __COMPONENT_H__


#include "entity.h"
#include "sprite.h"
#include "messages.h"
#include "game.h"
#include "windowmanager.h"
#include <functional>


class Component 
{
public:

	typedef std::function<void(GameMessage&)> MessageCallbackFun;
	typedef std::map<GameMessage::GM_Type, MessageCallbackFun> MessageCallbackMap;

	Component(Entity * owner) :
		mOwner(owner) {}
	virtual ~Component() {}

	Entity * mOwner;

	virtual void Init()               = 0;
	virtual void End()                = 0;
	virtual void Run(float deltaTime) = 0;
};


#endif

