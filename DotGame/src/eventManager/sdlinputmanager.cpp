#include "sdlinputmanager.h"
#include "globals.h"
#include "sdlwindowmanager.h"
#include "asserts.h"
#include <algorithm>
#include <utility>
#include "events.h"
#include "memorycontrol.h"
#include <SDL.h>


SdlInputManager * SdlInputManager::mInstance;

// *************************************************

SdlInputManager::SdlInputManager() :
	mInitialized (false) {}

// *************************************************

SdlInputManager::~SdlInputManager() {
	if (mInitialized)
		End();
}

// *************************************************

SdlInputManager * SdlInputManager::Instance() {
	if (!mInstance) {
		mInstance = GAME_NEW(SdlInputManager,());
		mInstance->Init();
	}

	return mInstance;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::Init() {

	if (mInitialized)
		return OK;

	m_pWindowManager = SdlWindowManager::Instance();

	GAME_ASSERT(m_pWindowManager);

	// Setting events
	// Mouse Move
	m_pWindowManager->SetMouseMoveCallback(&SdlInputManager::MouseMove);

	// Mouse Click
	m_pWindowManager->SetMouseClickCallback(&SdlInputManager::MouseClick);
	
	// Key Pressed
	m_pWindowManager->SetKeyPressedCallback(&SdlInputManager::KeyPressed);

	mInitialized = true;
	return OK;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::End() {

	return OK;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::RegisterEvent(IListener * listener, EM_Event e, int priority) {

	if (EM_Event::All == e)
		return KO;

	mListeners[e].insert(std::pair<int, IListener *>(priority, listener));

	return OK;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::UnregisterEvent(IListener * listener, EM_Event e) {

	if (EM_Event::All == e) {
		for (auto it = mListeners.begin(); it != mListeners.end(); ++it) {
			EM_Event event = it->first;
			RemoveListenerForEvent(listener, event);
		}
	}
	else {
		RemoveListenerForEvent(listener, e);
	}

	return OK;
}

// *************************************************

SdlInputManager::ListenerMap& SdlInputManager::GetListenerMap() {
	return mListeners;
}

// *************************************************

void SdlInputManager::RemoveListenerForEvent(IListener * listener, EM_Event e) {

	for(auto prioritiesIt = mListeners[e].begin(); prioritiesIt != mListeners[e].end(); ++prioritiesIt) {
		if (prioritiesIt->second == listener) {
			prioritiesIt = mListeners[e].erase(prioritiesIt);
			break;
		}
	}

}

// *************************************************

int SdlInputManager::MouseMove(void* userdata, SDL_Event* event) {
	ListenerMap& listenerMap = mInstance->GetListenerMap();

	if (listenerMap[EM_Event::MouseMove].size()) {
		for (auto prioritiesIt = listenerMap[EM_Event::MouseMove].begin(); prioritiesIt != listenerMap[EM_Event::MouseMove].end(); ++prioritiesIt) {
			prioritiesIt->second->ProcessEvent(EventMouseMove(static_cast<float>(xpos), static_cast<float>(ypos)));
		}
	}

	return 0;
}

// *************************************************

int SdlInputManager::MouseClick(void* userdata, SDL_Event* event) {
	ListenerMap& listenerMap = mInstance->GetListenerMap();

	EventMouseClick::EMouseButton mouseButton = EventMouseClick::EMouseButton::NotSupported;
	switch (event->type) {
		case GLFW_MOUSE_BUTTON_LEFT:   mouseButton = EventMouseClick::EMouseButton::Left;   break;
		case GLFW_MOUSE_BUTTON_MIDDLE: mouseButton = EventMouseClick::EMouseButton::Middle; break;
		case GLFW_MOUSE_BUTTON_RIGHT:  mouseButton = EventMouseClick::EMouseButton::Right;  break;
	}

	if (listenerMap[EM_Event::MouseClick].size()) {
		for (auto prioritiesIt = listenerMap[EM_Event::MouseClick].begin(); prioritiesIt != listenerMap[EM_Event::MouseClick].end(); ++prioritiesIt) {
			prioritiesIt->second->ProcessEvent(EventMouseClick(mouseButton));
		}
	}

	return 0;
}

// *************************************************

int SdlInputManager::KeyPressed(void* userdata, SDL_Event* event) {
	ListenerMap& listenerMap = mInstance->GetListenerMap();
	
	if (listenerMap[EM_Event::KeyPressed].size()) {
		for (auto prioritiesIt = listenerMap[EM_Event::KeyPressed].begin(); prioritiesIt != listenerMap[EM_Event::KeyPressed].end(); ++prioritiesIt) {
			prioritiesIt->second->ProcessEvent(EventKeyPressed(key));
		}
	}

	return 0;
}