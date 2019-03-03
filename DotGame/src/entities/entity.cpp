#include "globals.h"
#include "entity.h"
#include "component.h"
#include "memorycontrol.h"

Entity::Entity(Vec2 pos) :
	mPos(pos) {}

// *************************************************

Entity::Entity() :
	Entity(Vec2(0.f, 0.f)) {}

// *************************************************

Entity::~Entity()
{
	for (auto component : mComponents)
	{
		GAME_DELETE(component);
		component = nullptr;
	}
	mComponents.clear();
}

// *************************************************

void Entity::Run(float deltaTime)
{
	for(auto component : mComponents)
		component->Run(deltaTime);
}

// *************************************************

void Entity::AddComponent(Component * component)
{
	mComponents.push_back(component);
}

// *************************************************

void Entity::ReceiveMessage(GameMessage &message)
{
	for (auto component : mComponents)
	{
		IMessageReceiver * cReceiver = dynamic_cast<IMessageReceiver *>(component);
		if (cReceiver)
			cReceiver->ReceiveMessage(message);
	}
}

// *************************************************

Vec2 Entity::GetPos() const
{
	return mPos;
}

// *************************************************

void Entity::SetPos(Vec2 pos)
{
	mPos = pos;
}