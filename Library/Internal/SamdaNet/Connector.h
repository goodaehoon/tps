#pragma once

#include "EventObject.h"

#define MAX_ACCEPT_BUFFER 1024

class IOMultiplexer;
class SessionManager;
class IDispatcher;

class Connector : public EventObject
{
private:
	IOMultiplexer* ioMux;
	SessionManager* ssMgr;
	IDispatcher* dispatcher;

#ifdef _WINDOWS
	ConnectOverlapped connectOverlapped;
	char acceptBuffer[MAX_ACCEPT_BUFFER];
	Socket clientSocket;
#endif
	
public:
	Connector(IOMultiplexer* ioMux, SessionManager* ssMgr, IDispatcher* disp);
	
	bool Connect(const wchar_t* ip, unsigned short port);

#ifdef _WINDOWS
	bool BeginConnect();
#endif

	void OnConnect();
	void OnError(int err);
};

