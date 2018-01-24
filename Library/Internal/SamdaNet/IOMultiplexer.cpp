#include "Precompiled.h"
#include "IOMultiplexer.h"
#include "Acceptor.h"
#include "Session.h"
#include "SessionManager.h"

IOMultiplexer::IOMultiplexer()
#ifdef _WINDOWS
	: _iocp(INVALID_HANDLE_VALUE)
#else
	: _epfd(-1)
	, _events(nullptr)
#endif
{
}

IOMultiplexer::~IOMultiplexer()
{
	close();
}

bool IOMultiplexer::create()
{
#ifdef _WINDOWS
	_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	if (NULL == _iocp)
	{
		LOG_CORE(L"fail CreateIoCompletionPort(%u)", GetLastError());
		return false;
	}
#else
	_events = new epoll_event[MAX_EVENT];

	// Nowadays, size is ignored
	_epfd = epoll_create(maxevents);
	if (_epfd == -1)
	{
		LOG_CORE(L"fail epoll_create() error(%d): %s\n", errno, strerror(errno));
		return false;
	}

	LOG_CORE(L"epfd: %d\n", _epfd);
#endif
	return true;
}


void IOMultiplexer::close()
{
#ifdef _WINDOWS
	if (_iocp != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_iocp);
		_iocp = INVALID_HANDLE_VALUE;
	}
#else
	::close(_epfd);

	if (_events != nullptr)
		delete[] _events;
#endif
}

#ifdef _WINDOWS
bool IOMultiplexer::regist(SOCKET s)
{
	HANDLE h = CreateIoCompletionPort((HANDLE)s, _iocp, (unsigned long)s, 0);
	if (NULL == h)
	{
		LOG_CORE(L"fail regist CreateIoCompletionPort(%u)", GetLastError());
		return false;
	}

	return true;
}
#else
bool IOMultiplexer::regist(int fd, uint32_t events, void* ptr)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.events = events;
	ev.data.ptr = ptr;

	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) != 0)
	{
		LOG_CORE(L"fail epoll_ctl() _epfd(%d), fd(%d), ev.events(%u), ev.ptr(%p) error(%d): %s\n"
				, _epfd, fd, ev.events, ev.data.ptr, errno, strerror(errno));
		return false;
	}

	return true;
}
#endif

#ifdef _WINDOWS
bool IOMultiplexer::unregist(int fd)
{
	return true;
}
#else
bool IOMultiplexer::unregist(int fd)
{
	epoll_event ev;
	memset(&ev, 0, sizeof(ev));

	if (epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &ev) != 0)
	{
		LOG_CORE(L"fail epoll_ctl() _epfd(%d), fd(%d), error(%d): %s\n"
				, _epfd, fd, errno, strerror(errno));
		return false;
	}

	return true;
}
#endif

#ifdef _WINDOWS
void IOMultiplexer::waitForEvents()
{
	LPOVERLAPPED	pOverlapped = nullptr;
	unsigned long	transferred = 0;
	ULONG_PTR		key = 0;
	BOOL			result = FALSE;	

	result = GetQueuedCompletionStatus(_iocp, &transferred, &key, &pOverlapped, INFINITE);

	if (result == FALSE)
	{
		if (pOverlapped == NULL)
		{
			// 이 경우는 타임아웃(WAIT_TIMEOUT)이나 IOCP 핸들을 닫은 경우(ERROR_ABANDONED_WAIT_O) 발생한다
			if (GetLastError() != WAIT_TIMEOUT)
			{
				// IOCP 자체 에러인 경우로 더 이상 IOCP를 사용할 수 없는 경우이다
				LOG_CORE(L"pOverlapped is NULL (%u)\n", GetLastError());
			}			
		}
		else
		{
			// 입출력 장치의 작업과정에서 에러가 발생한 경우
			LOG_CORE(L"IO operation error (%u)\n", GetLastError());
		}

		return;
	}

	Overlapped* o = (Overlapped*)pOverlapped;
	o->Dispatch(result ? true : false, transferred);
}
#else
void IOMultiplexer::waitForEvents()
{
	int timeout = -1;
	int nfds = epoll_wait(_epfd, _events, MAX_EVENT, timeout);
	if (nfds == -1)
	{
		if (errno != EINTR)
		{
			LOG_CORE(L"fail epoll_wait() error(%d): %s\n", errno, strerror(errno));
			return;
		}
	}

	for (int i = 0; i < nfds; ++i)
	{
		EventObject* e = (EventObject*)_events[i].data.ptr;
		int events = _events[i].events;

		if (e->getType() == EVENT_OBJECT_ACCEPTOR)
		{
			if (events & EPOLLIN)
				e->onAccept();
			else if (events & (EPOLLERR | EPOLLHUP))
				e->onError(EVENT_ERROR_IO);
		}
		else if (e->getType() == EVENT_OBJECT_SESSION)
		{
			if (events & EPOLLIN)
				e->onRecv();
			else if (events & EPOLLOUT)
				e->onSend();
			else if (events & (EPOLLERR | EPOLLHUP))
				e->onError(EVENT_ERROR_IO);
		}
		else
		{
			assert(false);
		}
	}
}
#endif
