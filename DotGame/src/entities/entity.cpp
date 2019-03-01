#include "globals.h"
#include "entity.h"
#include "component.h"
#include "memorycontrol.h"


Entity::~Entity()
{
	for (auto component : m_components)
	{
		GAME_DELETE(component);
		component = nullptr;
	}
	m_components.clear();
}


void Entity::Run(float deltaTime)
{
	for(auto component : m_components)
		component->Run(deltaTime);
}

void Entity::AddComponent(Component * component)
{
	m_components.push_back(component);
}

void Entity::ReceiveMessage(GameMessage &message)
{
	for (auto component : m_components)
	{
		IMessageReceiver * cReceiver = dynamic_cast<IMessageReceiver *>(component);
		if (cReceiver)
			cReceiver->ReceiveMessage(message);
	}
}