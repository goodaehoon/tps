#pragma once

#include "Thread.h"

class SamdaNet;
class IDBDispatcher;

class DBThread : public Thread
{
private:
	SamdaNet& samdanet;
	IDBDispatcher* dbDispatcher;

public:
	DBThread(SamdaNet& samdanet);
	void SetDBDispatcher(IDBDispatcher* disp);

	void* Process();
};

