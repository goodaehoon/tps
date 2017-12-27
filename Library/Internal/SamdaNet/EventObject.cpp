#include "Precompiled.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Session.h"
#include "EventObject.h"

EventObject::EventObject()
	: type(EVENT_OBJECT_TYPE_NONE)
{
}

EventObject::~EventObject()
{
}

void EventObject::SetType(int type)
{
	type = type;
}

int EventObject::GetType()
{
	return type;
}

Socket& EventObject::GetSocket()
{
	return socket;
}

#ifdef _WINDOWS
Overlapped::Overlapped()
{
	memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
}

AcceptOverlapped::AcceptOverlapped(Acceptor* acceptor)
	: acceptor(acceptor)
{
}

void AcceptOverlapped::Reset()
{
	memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
}

void AcceptOverlapped::Dispatch(bool result, unsigned long transferred)
{
	acceptor->OnAccept();
}

RecvOverlapped::RecvOverlapped(Session* session)
	: session(session)
{
}

void RecvOverlapped::Reset()
{
	memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
}

void RecvOverlapped::Dispatch(bool result, unsigned long transferred)
{
	session->OnRecv(result, transferred);
}

SendOverlapped::SendOverlapped(Session* session)
	: session(session)
{
}

void SendOverlapped::Reset()
{
	memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
}

void SendOverlapped::Dispatch(bool result, unsigned long transferred)
{
	session->OnSend(result, transferred);
}
#endif

ConnectOverlapped::ConnectOverlapped(Connector* connector)
	: connector(connector)
{

}

void ConnectOverlapped::Reset()
{
	memset(static_cast<OVERLAPPED *>(this), 0, sizeof(OVERLAPPED));
}

void ConnectOverlapped::Dispatch(bool result, unsigned long transferred)
{
	connector->OnConnect();
}
