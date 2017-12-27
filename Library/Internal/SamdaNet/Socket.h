#pragma once

class Socket
{
private:
	SOCKET fd;

public:
	Socket();
	~Socket();

	void Clear();

	// 
	void SetFD(SOCKET s);

	//
	SOCKET GetFD();

	// create socket. using SOCK_STREAM or SOCK_DGRAM for type
	SOCKET Create(int type, int protocol);

	// 
	bool Bind(const wchar_t* ip, unsigned short port);

	//
	bool Listen();

	//
	bool Connect(const wchar_t* ip, unsigned short port);

	// 
	SOCKET Accept(sockaddr_in* in);

#ifdef _WINDOWS
	bool Recv(void* buf, size_t count, unsigned long* recvbytes, OVERLAPPED* overlap);
#endif

	//
	int Recv(void* buf, size_t count);


#ifdef _WINDOWS
	bool Send(void* buf, size_t count, unsigned long* sendbytes, OVERLAPPED* overlap);
#endif

	//
	int Send(const void* buf, size_t count);

	// change to non blocking socket
	bool Nonblock();

	// set reuse address 
	bool ReuseAddrs();

	// disable TIME-WAIT
	bool DisableTimeWait();

	// nagle algorithm off
	bool OffNagle();

#ifdef _WINDOWS
	bool InheritProperties(SOCKET parent);
#endif

	// 
	int GetSendBufSize();

	//
	int GetRecvBufSize();

	// 
	bool SetSendBufSize(int size);

	// 
	bool SetRecvBufSize(int size);
};

