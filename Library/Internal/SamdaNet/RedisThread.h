#pragma once

#include "Thread.h"

class SamdaNet;
class IRedisDispatcher;

class RedisThread : public Thread
{
private:
	SamdaNet& samdanet;
	IRedisDispatcher* redisDispatcher;

public:
	RedisThread(SamdaNet& samdanet);
	void SetRedisDispatcher(IRedisDispatcher* disp);

	void* Process();
};

