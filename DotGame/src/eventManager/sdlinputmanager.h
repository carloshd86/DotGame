#ifndef __SDL_INPUT_MANAGER_H__
#define __SDL_INPUT_MANAGER_H__

#include "eventmanager.h"
#include <map>
#include <vector>
#include "sdlwindowmanager.h"

struct SDL_Window;

class SdlInputManager : public IEventManager {
	
public:

	typedef std::map<EM_Event, std::multimap<int, IListener *>> ListenerMap;

	static SdlInputManager * Instance();
	~SdlInputManager();

	EM_Err Init();
	EM_Err End();

	EM_Err RegisterEvent   (IListener * listener, EM_Event e);
	EM_Err UnregisterEvent (IListener * listener, EM_Event e = EM_Event::All);

	ListenerMap& GetListenerMap();

	static int MouseMove  (void* userdata, SDL_Event* event);
	static int MouseClick (void* userdata, SDL_Event* event);
	static int KeyPressed (void* userdata, SDL_Event* event);

private:
	SdlInputManager();

	static SdlInputManager *mInstance;

	ListenerMap             mListeners;
	IWindowManager::Window *m_pWindow;
	SdlWindowManager       *m_pWindowManager;
	bool                    mInitialized;

	void RemoveListenerForEvent(IListener * listener, EM_Event e);
};

#endif