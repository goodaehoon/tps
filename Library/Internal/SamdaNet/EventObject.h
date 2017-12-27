#pragma once

#include "Socket.h"

enum EventObjectType
{
	EVENT_OBJECT_TYPE_NONE,
	EVENT_OBJECT_ACCEPTOR,
	EVENT_OBJECT_CONNECTOR,
	EVENT_OBJECT_SESSION,
};

class EventObject
{
protected:
	Socket socket;
	int type;

public:
	EventObject();
	virtual ~EventObject();

	Socket& GetSocket();

	void SetType(int type);
	int GetType();

	// �����쿡���� �Ʒ� �Լ��� ������� �ʴ´� (�Ű������� �ٸ����ؼ� ���� �������� �ʰ�, Overlapped Ŭ������ ó��)
	virtual void OnAccept() {};
	virtual void OnConnect() {};
	virtual void OnRecv() {};
	virtual void OnSend() {};
	virtual void OnError(int err) {};
};

#ifdef _WINDOWS
class Overlapped : public OVERLAPPED
{
public:
	Overlapped();

	virtual void Reset() {};
	virtual void Dispatch(bool result, unsigned long transferred) = 0;
};

class Acceptor;
class Connector;
class Session;

class AcceptOverlapped : public Overlapped
{
	Acceptor* acceptor;

public:
	AcceptOverlapped(Acceptor* acceptor);

	void Reset();
	void Dispatch(bool result, unsigned long transferred);
};

class ConnectOverlapped : public Overlapped
{
	Connector* connector;

public:
	ConnectOverlapped(Connector* connector);

	void Reset();
	void Dispatch(bool result, unsigned long transferred);
};

class RecvOverlapped : public Overlapped
{
	Session* session;

public:	
	RecvOverlapped(Session* session);

	void Reset();
	void Dispatch(bool result, unsigned long transferred);
};

class SendOverlapped : public Overlapped
{
	Session* session;

public:
	SendOverlapped(Session* session);

	void Reset();
	void Dispatch(bool result, unsigned long transferred);
};

#endif