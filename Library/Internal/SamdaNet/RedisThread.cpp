#include "Precompiled.h"
#include "RedisThread.h"
#include "SamdaNet.h"
#include "MessageQueue.h"
#include "Stream.h"
#include "Dispatcher.h"

RedisThread::RedisThread(SamdaNet& samdanet)
	: samdanet(samdanet)
	, redisDispatcher(nullptr)
{
}


void RedisThread::SetRedisDispatcher(IRedisDispatcher* disp)
{
	redisDispatcher = disp;
}

void* RedisThread::Process()
{
	MessageQueue* msgQueue = samdanet.GetMessageQueue();

	while(samdanet.IsRunning())
	{
		if (msgQueue->Empty(QueueType::REDIS_Q))
			continue;

		Stream* stream = msgQueue->Pop(QueueType::REDIS_Q);

		if (redisDispatcher)
			redisDispatcher->Dispatch((InterMsg*)stream);
	}

	return nullptr;
}
