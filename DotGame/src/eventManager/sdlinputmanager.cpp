#include "sdlinputmanager.h"
#include "globals.h"
#include "memorycontrol.h"
#include "events.h"
#include <SDL.h>
#include <algorithm>
#include <utility>


SdlInputManager * SdlInputManager::mInstance;

// *************************************************

SdlInputManager::SdlInputManager() :
	mInitialized   (false) {}

// *************************************************

SdlInputManager::~SdlInputManager() 
{
	if (mInitialized)
		End();
}

// *************************************************

SdlInputManager * SdlInputManager::Instance() 
{
	if (!mInstance) 
	{
		mInstance = GAME_NEW(SdlInputManager, ());
		mInstance->Init();
	}

	return mInstance;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::Init() 
{
	if (mInitialized)
		return OK;

	mInitialized = true;
	return OK;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::End() 
{
	return OK;
}

// ************************************************

void SdlInputManager::UpdateEvents() 
{
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) 
	{
		EM_Event eventType = GetEventFromSdlEventType(e.type);

		if (mListeners.find(eventType) != mListeners.end())
		{
			ListenerVector& listeners = mListeners[eventType];
			if (listeners.size())
			{
				switch (eventType)
				{
				case MouseClick:
				{
					SendMouseClick(e.button.button, listeners);
					break;
				}
				case MouseMove:
				{
					int x = 0, y = 0;
					SDL_GetMouseState(&x, &y);
					SendMouseMove(static_cast<float>(x), static_cast<float>(y), listeners);
					break;
				}
				case KeyPressed:
				{
					SendKeyPressed(e.key.keysym.sym, listeners);
					break;
				}
				case Quit:
				{
					SendQuit(listeners);
					gQuit = true;
					break;
				}
				default: break;
				}
			}
		}
	}

}

// *************************************************

IEventManager::EM_Err SdlInputManager::Register(IListener * listener, EM_Event e) 
{
	mListeners[e].push_back(listener);
	return OK;
}

// *************************************************

IEventManager::EM_Err SdlInputManager::Unregister(IListener * listener) 
{
	auto it = mListeners.begin();
	while (it != mListeners.end()) 
	{
		EM_Event event = it->first;
		RemoveListenerForEvent(listener, event);

		if (mListeners[event].empty())
			it = mListeners.erase(it);
		else
			++it;
	}

	return OK;
}

// *************************************************

SdlInputManager::ListenerMap& SdlInputManager::GetListenerMap() 
{
	return mListeners;
}

// *************************************************

void SdlInputManager::RemoveListenerForEvent(IListener * listener, EM_Event e) 
{
	for (auto it = mListeners[e].begin(); it != mListeners[e].end(); ++it)
	{
		if (*it == listener)
		{
			it = mListeners[e].erase(it);
			break;
		}
	}
}

// *************************************************ç

IEventManager::EM_Event SdlInputManager::GetEventFromSdlEventType(uint32_t type) const
{
	EM_Event event = None;
	switch (type) 
	{
		case SDL_MOUSEBUTTONDOWN: event = MouseClick; break;
		case SDL_MOUSEMOTION    : event = MouseMove;  break;
		case SDL_KEYUP          : event = KeyPressed; break;
		case SDL_QUIT           : event = Quit;       break;
		default                 : event = None;       break;
	}
	return event;
}


void SdlInputManager::SendMouseClick(uint8_t button, ListenerVector& listeners)
{
	EventMouseClick::EMouseButton mouseButton = EventMouseClick::EMouseButton::NotSupported;
	switch (button) {
	case SDL_BUTTON_LEFT  : mouseButton = EventMouseClick::EMouseButton::Left;   break;
	case SDL_BUTTON_MIDDLE: mouseButton = EventMouseClick::EMouseButton::Middle; break;
	case SDL_BUTTON_RIGHT : mouseButton = EventMouseClick::EMouseButton::Right;  break;
	}

	for (IListener* listener : listeners)
	{
		listener->ProcessEvent(EventMouseClick(mouseButton));
	}
}

// *************************************************ç

void SdlInputManager::SendMouseMove(float mouseX, float mouseY, ListenerVector& listeners)
{
	for (IListener* listener : listeners)
	{
		listener->ProcessEvent(EventMouseMove(mouseX, mouseY));
	}
}

// *************************************************ç

void SdlInputManager::SendKeyPressed(int32_t key, ListenerVector& listeners)
{
	for (IListener* listener : listeners)
	{
		listener->ProcessEvent(EventKeyPressed(key));
	}
}

// *************************************************ç

void SdlInputManager::SendQuit(ListenerVector& listeners) 
{
	for (IListener* listener : listeners)
	{
		listener->ProcessEvent(EventQuit());
	}
}