#pragma once

#include "Mutex.h"

class Session;
class IDispatcher;
class SamdaNet;

class SessionManager
{
	typedef unordered_map<unsigned int, Session* > Sessions;
private:	
	SamdaNet& samdanet;
	Sessions sessions;
	Mutex _syncObj;

public:
	SessionManager(SamdaNet& app);
	Session* Create(IDispatcher* disp);
	Session* GetSession(unsigned int sid);
	void Destroy(Session* s);
};

