#ifndef __COMPONENT_DOT_H__
#define __COMPONENT_DOT_H__

#include "component.h"
#include "eventmanager.h"

class CDot : public Component, public IEventManager::IListener
{
public:
	enum DotType
	{
		Green,
		Red
	};

	CDot(Entity* owner, DotType type, int tile);

	virtual ~CDot();

	void Init();
	void End();

	void Run(float deltaTime);

	DotType GetType() const;
	void SetType(DotType type);
	int GetTile() const;
	void SetTile(int tile);

private:
	bool    mInitialized;
	DotType mType;
	int     mTile;
};


#endif
