#pragma once

class IOMultiplexer;
class SessionManager;
class IDispatcher;
class Acceptor;

class AcceptorManager
{
	typedef list<Acceptor* > AcceptorList;

private:
	IOMultiplexer* ioMux;
	SessionManager* ssMgr;
	AcceptorList acceptors;

public:
	AcceptorManager(IOMultiplexer* ioMux, SessionManager* ssMgr);
	~AcceptorManager();

	Acceptor* create(IDispatcher* disp);
};

