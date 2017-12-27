#pragma once

#include "EventObject.h"

#define MAX_ACCEPT_BUFFER 1024

class IOMultiplexer;
class SessionManager;
class IDispatcher;

class Acceptor : public EventObject
{
private:
	IOMultiplexer* ioMux;
	SessionManager* ssMgr;
	IDispatcher* dispatcher;

#ifdef _WINDOWS
	AcceptOverlapped acceptOverlapped;
	char acceptBuffer[MAX_ACCEPT_BUFFER];
	Socket clientSocket;
#endif
	
public:
	Acceptor(IOMultiplexer* ioMux, SessionManager* ssMgr, IDispatcher* disp);
	
	bool Open(const wchar_t* ip, unsigned short port);

#ifdef _WINDOWS
	bool BeginAccept();
#endif

	void OnAccept();
	void OnError(int err);
};

