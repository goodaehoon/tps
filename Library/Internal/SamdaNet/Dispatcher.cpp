#include "Precompiled.h"
#include "Dispatcher.h"
#include "Session.h"
#include "ByteBuffer.h"
#include "Stream.h"
#include "Timer.h"
#include "PacketHeader.h"

//////////////////////////////////////////////////////////////////////////
// IDispatcher
//////////////////////////////////////////////////////////////////////////
bool IDispatcher::Parsing(Session* session, ByteBuffer* buffer)
{
	while (buffer->Length() >= (int)sizeof(PacketHeader))
	{
		PacketHeader* header = reinterpret_cast<PacketHeader* >(buffer->ReadPtr());

		if (header->GetPacketSize() >= buffer->Length())
		{
			Packet* new_packet = new Packet;
			buffer->Copy(new_packet->GetByteBuffer(), header->GetPacketSize());
			session->PutStream(new_packet);
		}
		else
		{
			break;
		}
	}

	return true;
}

void IDispatcher::Dispatch(Session* session, Packet* packet)
{
	PacketHeader* header = packet->GetHeaderPtr();
	packet_dispatcher handler = GetDispatcher(header->GetPacketNo());

	if (handler != nullptr)
	{
		handler(session, packet);
	}
}

void IDispatcher::OnConnected(Session* session)
{
}

void IDispatcher::OnDisconnected(Session* session, int err)
{
}

void IDispatcher::RegisterDispatcher(unsigned int id, packet_dispatcher dispatcher)
{
	packetDispatcher.insert(make_pair(id, dispatcher));
}

packet_dispatcher IDispatcher::GetDispatcher(unsigned int id)
{
	auto itr = packetDispatcher.find(id);
	if (itr == packetDispatcher.end())
		return nullptr;

	return itr->second;
}

//////////////////////////////////////////////////////////////////////////
// IDBDispatcher
//////////////////////////////////////////////////////////////////////////
void IDBDispatcher::RegisterDispatcher(unsigned int id, db_dispatcher dispatcher)
{
	dbDispatcher.insert(make_pair(id, dispatcher));
}

db_dispatcher IDBDispatcher::GetDispatcher(unsigned int id)
{
	auto itr = dbDispatcher.find(id);
	if (itr == dbDispatcher.end())
		return nullptr;

	return itr->second;
}

void IDBDispatcher::Dispatch(InterMsg* msg)
{
	InterMsgHeader* header = msg->GetHeaderPtr();
	db_dispatcher dispatcher = GetDispatcher(header->GetMsgNo());

	if (dispatcher != nullptr)
	{
		dispatcher(msg);
	}
}

//////////////////////////////////////////////////////////////////////////
// IInterDispatcher
//////////////////////////////////////////////////////////////////////////
void IInterDispatcher::RegisterDispatcher(unsigned int id, inter_dispatcher dispatcher)
{
	interDispatcher.insert(make_pair(id, dispatcher));
}

inter_dispatcher IInterDispatcher::GetDispatcher(unsigned int id)
{
	auto itr = interDispatcher.find(id);
	if (itr == interDispatcher.end())
		return nullptr;

	return itr->second;
}

void IInterDispatcher::Dispatch(Session* session, InterMsg* msg)
{
	InterMsgHeader* header = msg->GetHeaderPtr();
	inter_dispatcher dispatcher = GetDispatcher(header->GetMsgNo());

	if (dispatcher != nullptr)
	{
		dispatcher(session, msg);
	}
}

//////////////////////////////////////////////////////////////////////////
// ITimerDispatcher
//////////////////////////////////////////////////////////////////////////
void ITimerDispatcher::RegisterDispatcher(unsigned int id, timer_dispatcher dispatcher)
{
	timerDispatcher.insert(make_pair(id, dispatcher));
}

timer_dispatcher ITimerDispatcher::GetDispatcher(unsigned int id)
{
	auto itr = timerDispatcher.find(id);
	if (itr == timerDispatcher.end())
		return nullptr;

	return itr->second;
}

void ITimerDispatcher::Dispatch(Timer& timer)
{
	timer_dispatcher dispatcher = GetDispatcher(timer.timer_id);

	if (dispatcher != nullptr)
	{
		dispatcher(timer.object);
	}
}

//////////////////////////////////////////////////////////////////////////
// IRedisDispatcher
//////////////////////////////////////////////////////////////////////////
void IRedisDispatcher::RegisterDispatcher(unsigned int id, redis_dispatcher dispatcher)
{
	redisDispatcher.insert(make_pair(id, dispatcher));
}

redis_dispatcher IRedisDispatcher::GetDispatcher(unsigned int id)
{
	auto itr = redisDispatcher.find(id);
	if (itr == redisDispatcher.end())
		return nullptr;

	return itr->second;
}

void IRedisDispatcher::Dispatch(InterMsg* msg)
{
	InterMsgHeader* header = msg->GetHeaderPtr();
	redis_dispatcher dispatcher = GetDispatcher(header->GetMsgNo());

	if (dispatcher != nullptr)
	{
		dispatcher(msg);
	}
}