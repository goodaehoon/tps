#pragma once

#include "Dispatcher.h"

class RedisDispatcher : public IRedisDispatcher
{
public:
	RedisDispatcher();
	virtual ~RedisDispatcher() override;

	static int DISPATCHER_REDIS_LOGIN_REQ(InterMsg* msg);
};