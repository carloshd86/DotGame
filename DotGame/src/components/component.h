#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "entity.h"
#include "messages.h"
#include <functional>
#include <map>

typedef std::function<void(GameMessage&)> MessageCallbackFun;
typedef std::map<GameMessage::GM_Type, MessageCallbackFun> MessageCallbackMap;

class Component 
{
public:
	Component(Entity * owner) :
		mOwner(owner) {}

	virtual ~Component() {}

	virtual void Init()               = 0;
	virtual void End()                = 0;
	virtual void Run(float deltaTime) = 0;

protected:
	Entity * mOwner;
};


#endif

