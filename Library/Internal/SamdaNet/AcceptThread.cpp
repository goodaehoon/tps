#include "Precompiled.h"
#include "AcceptThread.h"
#include "SamdaNet.h"
#include "EventObject.h"
#include "Session.h"
#include "SessionManager.h"
#include "IOMultiplexer.h"

AcceptThread::AcceptThread(SamdaNet& samdanet)
	: _samdanet(samdanet)
	, _open(false)
{
}

#ifdef _WINDOWS
#else //_WINDOWS
bool AcceptThread::open(unsigned short port)
{
	return open(nullptr, port);
}

bool AcceptThread::open(const char* ip, unsigned short port)
{
	if (_sock.create(SOCK_STREAM, IPPROTO_TCP) == -1)
		return false;

	if (_sock.bind(ip, port) == false)
		return false;

	if (_sock.nonblock() == false)
		return false;

	if (_sock.reuseaddrs() == false)
		return false;

	if (_sock.listen() == false)
		return false;

	_open = true;

	return true;
}

void* AcceptThread::process()
{
	fd_set readfds;
	timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	int ret = 0;

	while(_samdanet.isRunning())
	{
		if (!_open)
			continue;

		FD_ZERO(&readfds);
		FD_SET(_sock.getSocket(), &readfds);
		ret = ::select(_sock.getSocket()+1, &readfds, 0, 0, &timeout);
		if (ret < 0)
			continue;

		if (!FD_ISSET(_sock.getSocket(), &readfds))
			continue;

		sockaddr_in addr;
		int fd = _sock.accept(&addr);
		if (fd < 0)
			continue;

		Session* s = _ssMgr->create(_disp);

		s->socket().setSocket(fd);

		LOG_CORE(L"ip = %s  port = %d", inet_ntoa(addr.sin_addr), ntohs((short)addr.sin_port));
	}

	return nullptr;
}

#endif //_WINDOWS