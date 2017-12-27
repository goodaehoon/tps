#include "Precompiled.h"
#include "Socket.h"

Socket::Socket()
	: fd(INVALID_SOCKET)
{
	
}

Socket::~Socket()
{
#ifdef _WINDOWS
	closesocket(fd);
#else
	close(fd);
#endif
}

void Socket::Clear()
{
	fd = INVALID_SOCKET;
}

void Socket::SetFD(SOCKET s)
{
	fd = s;
}

SOCKET Socket::GetFD()
{
	return fd;
}

SOCKET Socket::Create(int type, int protocol)
{
#ifdef _WINDOWS
	fd = WSASocket(AF_INET, type, protocol, 0, 0, WSA_FLAG_OVERLAPPED);
	if (fd == INVALID_SOCKET)
		LOG_CORE(L"fail socket() err(%d)\n", WSAGetLastError());
#else
	fd = socket(AF_INET, type, 0);
	if (fd == -1)
		LOG_CORE(L"fail socket() err(%d) : %s\n", errno, strerror(errno));
#endif
	
	return fd;
}

bool Socket::Bind(const wchar_t* ip, unsigned short port)
{
	sockaddr_in in;
	in.sin_family = AF_INET;
	in.sin_port = htons(port);

	if (ip == nullptr || wcscmp(ip, L""))
	{
		in.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		char ip_addr[64] = { 0, };
		wcstombs(ip_addr, ip, wcslen(ip));
		in.sin_addr.s_addr = inet_addr(ip_addr);
	}

	if (::bind(fd, (sockaddr*)&in, sizeof(in)) != 0)
	{
		LOG_CORE(L"fail bind() err(%d) : %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

bool Socket::Listen()
{
	const int backlog = 5;
	if (::listen(fd, backlog) != 0)
	{
		LOG_CORE(L"fail listen() err(%d) : %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

bool Socket::Connect(const wchar_t* ip, unsigned short port)
{
	return true;
}

SOCKET Socket::Accept(sockaddr_in* addr)
{
	sockaddr_in in;
	socklen_t len = sizeof(in);
	SOCKET s = ::accept(fd, (sockaddr *)&in, &len);
	if (s == -1)
	{
		LOG_CORE(L"fail accept() error(%d): %s\n", errno, strerror(errno));
		return -1;
	}

	if (addr != nullptr)
		memcpy(addr, &in, sizeof(sockaddr));
	
	return s;
}

int Socket::Recv(void* buf, size_t count)
{
#ifdef _WINDOWS
	int rbytes = ::recv(fd, (char *)buf, (int) count, 0);
#else
	ssize_t rbytes = read(fd, buf, count);
#endif
	if (rbytes == -1)
	{
#ifdef _WINDOWS
		if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAEINTR)
#else //_WINDOWS
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
#endif //_WINDOWS
			return 0;
		else
			return -1;
	}
	else if (rbytes == 0)
	{
		return -1;
	}

	return rbytes;
}

#ifdef _WINDOWS
bool Socket::Recv(void* buf, size_t count, unsigned long* recvbytes, OVERLAPPED* overlap)
{
	WSABUF wsabuf;
	wsabuf.buf = (char*)buf;
	wsabuf.len = (ULONG)count;
	unsigned long flags = 0;

	int ret = WSARecv(fd, &wsabuf, 1, recvbytes, &flags, overlap, 0);

	int lasterr = WSAGetLastError();
	if (ret == SOCKET_ERROR)
	{
		if ((lasterr != WSAEWOULDBLOCK) && (lasterr != WSA_IO_PENDING))
			return false;
	}

	return true;
}

bool Socket::Send(void* buf, size_t count, unsigned long* sendbytes, OVERLAPPED* overlap)
{
	WSABUF wsabuf;
	wsabuf.buf = (char*)buf;
	wsabuf.len = (ULONG)count;

	int ret = WSASend(fd, &wsabuf, 1, sendbytes, 0, overlap, 0);

	int lasterr = WSAGetLastError();
	if (ret == SOCKET_ERROR)
	{
		if ((lasterr != WSAEWOULDBLOCK) && (lasterr != WSA_IO_PENDING))
			return false;
	}

	return true;
}
#endif

int Socket::Send(const void* buf, size_t count)
{
#ifdef _WINDOWS
	int sbytes = ::send(fd, (char *)buf, (int)count, 0);
#else
	ssize_t sbytes = write(fd, buf, count);
#endif
	if (sbytes == -1)
	{
#ifdef _WINDOWS
		if (WSAGetLastError() == WSAEWOULDBLOCK)
#else
		if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
#endif
			return 0;
		else
			return -1;
	}

	return sbytes;
}

bool Socket::Nonblock()
{
#ifdef _WINDOWS
	u_long flag = 1;
	ioctlsocket(fd, FIONBIO, &flag);
#else
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
	{
		LOG_CORE(L"fail fcntl(F_GETFL) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1)
	{
		LOG_CORE(L"fail fcntl(F_SETFL) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
#endif

	return true;
}
	
bool Socket::ReuseAddrs()
{
	int optval = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval)) != 0)
	{
		LOG_CORE(L"fail setsockopt(SO_REUSEADDR) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::DisableTimeWait()
{
	struct linger l;
	l.l_onoff = 1;
	l.l_linger = 0;

#ifdef _WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*)&l, sizeof(l)) != 0)
#else
	if (setsockopt(fd, SOL_SOCKET, SO_LINGER, (void*)&l, sizeof(l)) != 0)
#endif
	{
		LOG_CORE(L"fail setsockopt(SO_LINGER) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::OffNagle()
{
	int optval = 0;
#ifdef _WINDOWS
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval)) != 0)
#else
	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval)) != 0)
#endif
	{
		LOG_CORE(L"fail setsockopt(TCP_NODELAY) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

#ifdef _WINDOWS
bool Socket::InheritProperties(SOCKET parent)
{
	if (setsockopt(fd, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (const char*)&parent, sizeof(parent)) != 0)
	{
		LOG_CORE(L"fail setsockopt(SO_UPDATE_ACCEPT_CONTEXT) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}
#endif

int Socket::GetSendBufSize()
{
	int bufsize = 0;
#ifdef _WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //_WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //_WINDOWS
	{
		LOG_CORE(L"fail setsockopt(SO_SNDBUF) err(%d) : %s\n", errno, strerror(errno));
		return -1;
	}
	
	return bufsize;
}

int Socket::GetRecvBufSize()
{
	int bufsize = 0;
#ifdef _WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //_WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //_WINDOWS
	{
		LOG_CORE(L"fail setsockopt(SO_REVBUF) err(%d) : %s\n", errno, strerror(errno));
		return -1;
	}
	
	return bufsize;
}

bool Socket::SetSendBufSize(int size)
{
	int bufsize = size;
#ifdef _WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //_WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //_WINDOWS
	{
		LOG_CORE(L"fail setsockopt(SO_SNDBUF) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}

bool Socket::SetRecvBufSize(int size)
{
	int bufsize = size;
#ifdef _WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (const char*)&bufsize, sizeof(bufsize)) != 0)
#else //_WINDOWS
	if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (void*)&bufsize, sizeof(bufsize)) != 0)
#endif //_WINDOWS
	{
		LOG_CORE(L"fail setsockopt(SO_REVBUF) err(%d) : %s\n", errno, strerror(errno));
		return false;
	}
	
	return true;
}



