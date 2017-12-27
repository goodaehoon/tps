#pragma once

#include "Dispatcher.h"

class ClientDispatcher : public IDispatcher
{
public:
	ClientDispatcher();
	virtual ~ClientDispatcher() override;

	virtual void OnConnected(Session* session) override;
	virtual void OnDisconnected(Session* session, int err) override;

	static int DISPATCHER_LOGIN_REQUEST(Session* session, Packet* packet);
};

