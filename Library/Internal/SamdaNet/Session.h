#pragma once

#include "EventObject.h"
#include "MemoryPool.h"

class MessageQueue;
class SessionManager;
class IDispatcher;
class ByteBuffer;
class Packet;
class Stream;
struct PacketHeader;

class Session 
	: public EventObject
	, public MemoryPool<Session, MAX_SESSION_COUNT>
{
private:
	MessageQueue* msgQueue;
	SessionManager* sessMgr;
	IDispatcher* dispatcher;
	ByteBuffer* recvBuf;
	ByteBuffer* sendBuf;

#ifdef _WINDOWS
	RecvOverlapped recvOverlapped;
	SendOverlapped sendOverlapped;
#endif

public:
	Session();
	~Session();

	void Clear();
	void SetSessionManager(SessionManager* mgr);
	void SetMessageQueue(MessageQueue* mgr);
	void SetDispatcher(IDispatcher* disp);	
	void OnConnect();
	void Dispatch(Packet* packet);
	void CloseSession();
	void PutStream(Stream* s);
	void Send(Packet& packet);
	void Send(PacketHeader* packet);

#ifdef _WINDOWS
	bool WaitForRecv();

	bool WaitForSend();

	void OnRecv(bool result, unsigned long recvbytes);

	void OnSend(bool result, unsigned long sendbytes);
#else
	void OnRecv();

	void OnSend();

	void OnError(int errtype);
#endif	
};

