#pragma once

#include "Mutex.h"
#include <queue>

class Stream;
class ByteBuffer;
struct InterMsgHeader;

enum QueueType
{
	PACKET_Q,
	DB_Q,
	INTER_MSG_Q,
	REDIS_Q,
	Q_TYPE_MAX,
};

class MessageQueue
{
private:
	queue<Stream* > streamQueue[QueueType::Q_TYPE_MAX];
	Mutex syncObj[QueueType::Q_TYPE_MAX];

public:
	void Push(QueueType type, Stream* s);
	void Push(QueueType type, InterMsgHeader* inter_msg);
	Stream* Pop(QueueType type);
	size_t Size(QueueType type);
	bool Empty(QueueType type);
};

