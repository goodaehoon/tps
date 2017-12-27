#include "Precompiled.h"
#include "NetworkThread.h"
#include "SamdaNet.h"
#include "EventObject.h"
#include "IOMultiplexer.h"

NetworkThread::NetworkThread(SamdaNet& samdanet)
	: samdanet(samdanet)
{
}

void* NetworkThread::Process()
{
	while(samdanet.IsRunning())
	{
		samdanet.GetIOMultiplexer()->waitForEvents();
	}

	return nullptr;
}
