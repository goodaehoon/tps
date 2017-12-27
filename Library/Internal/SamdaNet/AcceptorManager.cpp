#include "Precompiled.h"
#include "AcceptorManager.h"
#include "IOMultiplexer.h"
#include "SessionManager.h"
#include "Dispatcher.h"
#include "Acceptor.h"

AcceptorManager::AcceptorManager(IOMultiplexer* ioMux, SessionManager* ssMgr)
	: ioMux(ioMux)
	, ssMgr(ssMgr)
{
}

AcceptorManager::~AcceptorManager()
{
	for (auto itr : acceptors)
	{
		SAFE_DELETE(itr);
	}
}

Acceptor* AcceptorManager::create(IDispatcher* disp)
{
	Acceptor* acceptor = new Acceptor(ioMux, ssMgr, disp);

	acceptor->SetType(EVENT_OBJECT_ACCEPTOR);

	acceptors.push_back(acceptor);

	return acceptor;
}

