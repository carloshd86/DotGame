#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__


class IEventManager 
{

public:
	enum EM_Err  
	{
		KO = 0,
		OK = 1
	};

	enum EM_Event 
	{
		KeyPressed,
		MouseClick,
		MouseMove,
		WindowFocusChanged,
		All
	};

	class IListener 
	{
	public:
		virtual bool ProcessEvent (EM_Event event) = 0;
	};

	virtual void   UpdateEvents ()                                             = 0;
	virtual EM_Err Register   (IListener * listener, EM_Event e, int priority) = 0;
	virtual EM_Err Unregister (IListener * listener)                           = 0;
};

#endif