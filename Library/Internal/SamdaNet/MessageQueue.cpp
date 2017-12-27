#include "Precompiled.h"
#include "MessageQueue.h"
#include "ByteBuffer.h"
#include "InterMsgHeader.h"
#include "Stream.h"

void MessageQueue::Push(QueueType type, Stream* s)
{
	LockGuard<Mutex> lock(syncObj[type]);

	streamQueue[type].push(s);
}

void MessageQueue::Push(QueueType type, InterMsgHeader* inter_msg)
{
	LockGuard<Mutex> lock(syncObj[type]);

	InterMsg* s = new InterMsg;
	s->PutBinary((char*)inter_msg, inter_msg->GetMsgSize());

	streamQueue[type].push(s);
}

Stream*  MessageQueue::Pop(QueueType type)
{
	LockGuard<Mutex> lock(syncObj[type]);

	if (streamQueue[type].empty() == true)
		return nullptr;

	Stream* s = streamQueue[type].front();
	streamQueue[type].pop();

	return s;	
}

size_t MessageQueue::Size(QueueType type)
{
	LockGuard<Mutex> lock(syncObj[type]);

	return streamQueue[type].size();
}

bool MessageQueue::Empty(QueueType type)
{
	LockGuard<Mutex> lock(syncObj[type]);

	return streamQueue[type].empty();
}