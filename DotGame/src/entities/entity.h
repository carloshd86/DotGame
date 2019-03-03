#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "core.h"
#include "messagereceiver.h"
#include <vector>

class Component;

class Entity : public IMessageReceiver
{
public:

	~Entity();
	Entity();
	Entity(Vec2 pos);

	virtual void Run          (float deltaTime);
	void         AddComponent (Component * component);

	Vec2 GetPos() const;
	void SetPos(Vec2 pos);

	virtual void ReceiveMessage (GameMessage &message);

private:

	Vec2                     mPos;
	std::vector<Component *> mComponents;
};

#endif