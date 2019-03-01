#ifndef __EVENTS_H__
#define __EVENTS_H__

#include "eventmanager.h"

class Event {

public:
	Event(IEventManager::EM_Event type) : mType(type) {}
	virtual ~Event() {}

	IEventManager::EM_Event GetType() const { return mType; }

private:
	IEventManager::EM_Event mType;
};

// *************************************************

class EventKeyPressed : public Event {

public:
	EventKeyPressed(int key) :
		Event(IEventManager::EM_Event::KeyPressed),
		mKey(key) {}

	int GetKey() const { return mKey; }

private:
	int mKey;
};

// *************************************************

class EventMouseClick : public Event {

public:
	enum EMouseButton {
		Left,
		Right,
		Middle,
		NotSupported
	};

	EventMouseClick(EMouseButton mouseButton) :
		Event(IEventManager::EM_Event::MouseClick),
		mMouseButton(mouseButton) {}

	EMouseButton GetMouseButton() const { return mMouseButton; }

private:
	EMouseButton mMouseButton;
};

// *************************************************

class EventMouseMove : public Event {

public:
	EventMouseMove(float posX, float posY) :
		Event(IEventManager::EM_Event::MouseMove),
		mPosX(posX),
		mPosY(posY) {}

	float GetPosX() const { return mPosX; }
	float GetPosY() const { return mPosY; }

private:
	float mPosX;
	float mPosY;
};

// *************************************************

class EventWindowFocusChanged : public Event {

public:
	EventWindowFocusChanged(bool focused) :
		Event(IEventManager::EM_Event::WindowFocusChanged),
		mFocused(focused) {}

	bool GetFocused() const { return mFocused; }

private:
	bool mFocused;
};

#endif