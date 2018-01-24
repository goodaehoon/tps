#include "Precompiled.h"
#include "NetworkThread.h"
#include "SamdaNet.h"
#include "EventObject.h"
#include "IOMultiplexer.h"

NetworkThread::NetworkThread()
	: pNetEngine(nullptr)
{
}

NetworkThread::NetworkThread(SamdaNet* pNet)
	: pNetEngine(pNet)
{
}

void NetworkThread::SetNetEngine(SamdaNet* pNet)
{
	pNetEngine = pNet;
}

void* NetworkThread::Process()
{
	while(pNetEngine->IsRunning())
	{
		pNetEngine->GetIOMultiplexer()->waitForEvents();
	}

	return nullptr;
}
