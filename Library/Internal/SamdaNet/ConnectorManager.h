#pragma once

class IOMultiplexer;
class SessionManager;
class IDispatcher;
class Connector;

class ConnectorManager
{
	typedef list<Connector* > ConnectorList;

private:
	IOMultiplexer* ioMux;
	SessionManager* ssMgr;
	ConnectorList connectors;

public:
	ConnectorManager(IOMultiplexer* ioMux, SessionManager* ssMgr);
	~ConnectorManager();

	Connector* create(IDispatcher* disp);
};

