#pragma once

#include "Thread.h"

class SamdaNet;
class IInterDispatcher;
class ITimerDispatcher;

class MainThread : public Thread
{
private:
	SamdaNet& samdanet;
	IInterDispatcher* interDispatcher;
	ITimerDispatcher* timerDispatcher;

public:
	MainThread(SamdaNet& samdanet);
	void SetInterDispatcher(IInterDispatcher* disp);
	void SetTimerDispatcher(ITimerDispatcher* disp);

	void* Process();
};

