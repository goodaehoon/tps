#include "Precompiled.h"
#include "Session.h"
#include "SessionManager.h"
#include "Socket.h"
#include "Dispatcher.h"
#include "Stream.h"
#include "ByteBuffer.h"
#include "PacketHeader.h"

Session::Session()
	: sessMgr(nullptr)
	, dispatcher(nullptr)
	, recvBuf(new ByteBuffer(MAX_RECV_BUFFER))
	, sendBuf(new ByteBuffer(MAX_SEND_BUFFER))
#ifdef _WINDOWS
	, recvOverlapped(this)
	, sendOverlapped(this)
#endif
{
}

Session::~Session()
{
	Clear();

	SAFE_DELETE(recvBuf);
	SAFE_DELETE(sendBuf);
}

void Session::Clear()
{
	dispatcher = nullptr;
}

void Session::SetMessageQueue(MessageQueue* mgr)
{
	msgQueue = mgr;
}

void Session::SetSessionManager(SessionManager* mgr)
{
	sessMgr = mgr;
}

void Session::SetDispatcher(IDispatcher* disp)
{
	dispatcher = disp;
}

void Session::OnConnect()
{
	dispatcher->OnConnected(this);
}

void Session::PutStream(Stream* s)
{
	s->SetCallerID(GetID());

	msgQueue->Push(QueueType::PACKET_Q, s);
}

void Session::Dispatch(Packet* packet)
{
	dispatcher->Dispatch(this, packet);
}

void Session::CloseSession()
{
	dispatcher->OnDisconnected(this, errno);
	closesocket(socket.GetFD());
	sessMgr->Destroy(this);
}

void Session::Send(Packet& packet)
{
	packet.EndPacket();

	ByteBuffer* buffer = packet.GetByteBuffer();
	socket.Send(buffer->ReadPtr(), buffer->Length());
}

void Session::Send(PacketHeader* fixed_packet)
{
	Packet packet;
	packet.PutBinary((char*)fixed_packet, fixed_packet->packetSize);
	packet.EndPacket();

	ByteBuffer* buffer = packet.GetByteBuffer();
	socket.Send(buffer->ReadPtr(), buffer->Length());
}


#ifdef _WINDOWS
bool Session::WaitForRecv()
{
	recvOverlapped.Reset();

	unsigned long recvbytes = 0;

	return socket.Recv((void*)recvBuf->WritePtr(), MAX_RECV_BUFFER, &recvbytes, static_cast<OVERLAPPED *>(&recvOverlapped));
}

void Session::OnRecv(bool result, unsigned long recvbytes)
{
	// Recv작업이 실패하거나 0 (상대방이 끊을 경우)에 접속종료 처리를 한다
	if (result == false || recvbytes == 0)
	{
		CloseSession();
		return;
	}

	// Recv작업이 성공하면 버퍼에 옮겨준다
	//memcpy(recvBuf->WritePtr(), recvBuf->ReadPtr(), recvbytes);
	recvBuf->Write(recvbytes);

	if (dispatcher->Parsing(this, recvBuf) == false)
	{
		OnError(EVENT_ERROR_PARSING);
		return;
	}

	WaitForRecv();
}

bool Session::WaitForSend()
{
	sendOverlapped.Reset();

	unsigned long sendbytes = 0;

	return socket.Send((void*)sendBuf->ReadPtr(), sendBuf->Length(), &sendbytes, static_cast<OVERLAPPED *>(&sendOverlapped));
}

void Session::OnSend(bool result, unsigned long sendbytes)
{
	LOG_CORE(L"%d %d\n", result, sendbytes);
}
#else
void Session::OnRecv()
{
	int nrecv = socket.recv(recvBuf->WritePtr(), recvBuf->remain());
	if (nrecv < 0)
	{	
		OnError(errno);
		return;	
	}
	else if (nrecv == 0)
	{
		OnError(0);
		return;
	}

	recvBuf->write(nrecv);

	if (dispatcher->parsing(recvBuf) == false)
	{
		OnError(EVENT_ERROR_PARSING);
		return;
	}
}

void Session::OnSend()
{
//	int nsend = socket.send(sendBuf->ReadPtr(), sendBuf->Length());
//	if (nsend < 0)
	{
		OnError(errno);
		return;
	}

//	sendBuf->read(nsend);
}

void Session::OnError(int errtype)
{
	dispatcher->onDisconnect(errtype);
}
#endif
