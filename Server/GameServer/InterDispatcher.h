#pragma once

#include "Dispatcher.h"

class InterDispatcher : public IInterDispatcher
{
public:
	InterDispatcher();
	virtual ~InterDispatcher() override;

	static int DISPATCHER_INTER_LOGIN_RESPONSE(Session* session, InterMsg* msg);
};