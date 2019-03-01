#ifndef __MESSAGE_RECEIVER_H__
#define __MESSAGE_RECEIVER_H__


class GameMessage;

class IMessageReceiver 
{
public:

	virtual void ReceiveMessage(GameMessage &message) = 0;
};

#endif