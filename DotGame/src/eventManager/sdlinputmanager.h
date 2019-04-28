#ifndef __SDL_INPUT_MANAGER_H__
#define __SDL_INPUT_MANAGER_H__

#include "eventmanager.h"
#include <map>
#include <vector>
#include "sdlwindowmanager.h"

struct SDL_Window;

class SdlInputManager : public IEventManager {
public:

	typedef std::vector<IListener*>            ListenerVector;
	typedef std::map<EM_Event, ListenerVector> ListenerMap;

	static SdlInputManager * Instance();
	~SdlInputManager();

	EM_Err Init();
	EM_Err End();

	void   UpdateEvents ();
	EM_Err Register     (IListener * listener, EM_Event e);
	EM_Err Unregister   (IListener * listener);

	ListenerMap& GetListenerMap();

private:
	SdlInputManager();

	static SdlInputManager *mInstance;

	ListenerMap                mListeners;
	bool                       mInitialized;

	void RemoveListenerForEvent      (IListener* listener, EM_Event e);
	EM_Event GetEventFromSdlEventType(uint32_t type) const;

	void SendMouseClick(uint8_t mouseButton, ListenerVector& listeners);
	void SendMouseMove (float mouseX, float mouseY, ListenerVector& listeners);
	void SendKeyPressed(int32_t key, ListenerVector& listeners);
	void SendQuit(ListenerVector& listeners);
};

#endif