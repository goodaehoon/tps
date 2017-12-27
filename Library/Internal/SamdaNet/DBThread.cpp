#include "Precompiled.h"
#include "DBThread.h"
#include "SamdaNet.h"
#include "MessageQueue.h"
#include "Stream.h"
#include "Dispatcher.h"

DBThread::DBThread(SamdaNet& samdanet)
	: samdanet(samdanet)
	, dbDispatcher(nullptr)
{
}


void DBThread::SetDBDispatcher(IDBDispatcher* disp)
{
	dbDispatcher = disp;
}

void* DBThread::Process()
{
	MessageQueue* msgQueue = samdanet.GetMessageQueue();

	while(samdanet.IsRunning())
	{
		if (msgQueue->Empty(QueueType::DB_Q))
			continue;

		Stream* stream = msgQueue->Pop(QueueType::DB_Q);

		if (dbDispatcher)
			dbDispatcher->Dispatch((InterMsg*)stream);
	}

	return nullptr;
}
