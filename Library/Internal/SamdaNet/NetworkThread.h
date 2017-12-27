#pragma once

#include "Thread.h"

class SamdaNet;

class NetworkThread : public Thread
{
private:
	SamdaNet& samdanet;

public:
	NetworkThread(SamdaNet& samdanet);

	void* Process();
};

