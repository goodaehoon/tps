#pragma once

#define MAX_EVENT 10240

class IOMultiplexer
{
private:

#ifdef _WINDOWS
	HANDLE _iocp;
#else
	int _epfd;
	int _maxevents;
	epoll_event* _events;
#endif

public:
	IOMultiplexer();
	~IOMultiplexer();

	bool create();

	void close();

#ifdef _WINDOWS
	bool regist(SOCKET s);
	bool unregist(int fd);	
#else
	bool regist(int fd, uint32_t events, void* ptr);	
	bool unregist(int fd);	
#endif

	void waitForEvents();
};

