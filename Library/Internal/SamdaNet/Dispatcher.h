#pragma once

#include "Session.h"
#include "MessageQueue.h"

class ByteBuffer;
class Packet;
class Session;
class InterMsg;
class Timer;

//////////////////////////////////////////////////////////////////////////
// IDispatcher
//////////////////////////////////////////////////////////////////////////
typedef int (*packet_dispatcher)(Session* session, Packet* packet);

class IDispatcher
{
	typedef unordered_map<unsigned int, packet_dispatcher> PacketDispatcher;

protected:
	PacketDispatcher packetDispatcher;

public:
	IDispatcher() {};
	virtual ~IDispatcher() {};
	virtual bool Parsing(Session* session, ByteBuffer* buffer);
	virtual void OnConnected(Session* session);
	virtual void OnDisconnected(Session* session, int err);

	void Dispatch(Session* session, Packet* packet);
	void RegisterDispatcher(unsigned int id, packet_dispatcher handler);
	packet_dispatcher GetDispatcher(unsigned int id);
};


//////////////////////////////////////////////////////////////////////////
// IDBDispatcher
//////////////////////////////////////////////////////////////////////////
typedef int(*db_dispatcher)(InterMsg* msg);

class IDBDispatcher
{
	typedef unordered_map<unsigned int, db_dispatcher> DBDispatcher;

protected:
	DBDispatcher dbDispatcher;

public:
	IDBDispatcher() {};
	virtual ~IDBDispatcher() {};

	virtual void Dispatch(InterMsg* msg);
	void RegisterDispatcher(unsigned int id, db_dispatcher handler);
	db_dispatcher GetDispatcher(unsigned int id);
};


//////////////////////////////////////////////////////////////////////////
// IInterDispatcher
//////////////////////////////////////////////////////////////////////////
typedef int(*inter_dispatcher)(Session* session, InterMsg* msg);

class IInterDispatcher
{
	typedef unordered_map<unsigned int, inter_dispatcher> InterDispatcher;

protected:
	InterDispatcher interDispatcher;

public:
	IInterDispatcher() {};
	virtual ~IInterDispatcher() {};

	virtual void Dispatch(Session* session, InterMsg* msg);
	void RegisterDispatcher(unsigned int id, inter_dispatcher handler);
	inter_dispatcher GetDispatcher(unsigned int id);
};

//////////////////////////////////////////////////////////////////////////
// ITimerDispatcher
//////////////////////////////////////////////////////////////////////////
typedef int(*timer_dispatcher)(void* object);

class ITimerDispatcher
{
	typedef unordered_map<unsigned int, timer_dispatcher> TimerDispatcher;

protected:
	TimerDispatcher timerDispatcher;

public:
	ITimerDispatcher() {};
	virtual ~ITimerDispatcher() {};

	virtual void Dispatch(Timer& timer);
	void RegisterDispatcher(unsigned int id, timer_dispatcher handler);
	timer_dispatcher GetDispatcher(unsigned int id);
};

//////////////////////////////////////////////////////////////////////////
// IRedisDispatcher
//////////////////////////////////////////////////////////////////////////
typedef int(*redis_dispatcher)(InterMsg* msg);

class IRedisDispatcher
{
	typedef unordered_map<unsigned int, redis_dispatcher> DBDispatcher;

protected:
	DBDispatcher redisDispatcher;

public:
	IRedisDispatcher() {};
	virtual ~IRedisDispatcher() {};

	virtual void Dispatch(InterMsg* msg);
	void RegisterDispatcher(unsigned int id, redis_dispatcher handler);
	redis_dispatcher GetDispatcher(unsigned int id);
};