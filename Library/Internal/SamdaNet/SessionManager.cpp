#include "Precompiled.h"
#include "SessionManager.h"
#include "Session.h"
#include "SamdaNet.h"
#include "IOMultiplexer.h"

SessionManager::SessionManager(SamdaNet& app)
	: samdanet(app)
{
}

// IOCP Worker Threadø°º≠ »£√‚µ 
Session* SessionManager::Create(IDispatcher* disp)
{
	Session* s = new Session();
	
	s->SetType(EVENT_OBJECT_SESSION);
	s->SetSessionManager(this);
	s->SetMessageQueue(samdanet.GetMessageQueue());
	s->SetDispatcher(disp);

	{
		LockGuard<Mutex> lock(_syncObj);

		sessions.insert(make_pair(s->GetID(), s));
	}
	
	return s;
}

// MainThreadø°º≠ »£√‚µ 
Session* SessionManager::GetSession(unsigned int sid)
{
	LockGuard<Mutex> lock(_syncObj);

	auto itr = sessions.find(sid);
	if (itr != sessions.end())
	{
		return itr->second;
	}

	return nullptr;
}

void SessionManager::Destroy(Session* s)
{
	if (s == nullptr)
		return;

	s->Clear();

	{
		LockGuard<Mutex> lock(_syncObj);

		sessions.erase(s->GetID());
	}

	delete s;
}
