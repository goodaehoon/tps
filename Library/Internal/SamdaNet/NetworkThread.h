#pragma once

#include "Thread.h"

class SamdaNet;

class NetworkThread : public Thread
{
private:
	SamdaNet* pNetEngine;

public:
	NetworkThread();
	NetworkThread(SamdaNet* pNet);

	void SetNetEngine(SamdaNet* pNet);
	void* Process();
};

