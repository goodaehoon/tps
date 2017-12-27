#include "Precompiled.h"
#include "TimerDispatcher.h"
#include "Stream.h"
#include "InterPacket.h"
#include "MySQLConnector.h"
#include "DBManager.h"
#include "SamdaNet.h"
#include "StringUtil.h"
#include "MessageQueue.h"
#include "Application.h"
#include "Log.h"

using namespace InterPacket;

TimerDispatcher::TimerDispatcher()
{
	RegisterDispatcher(1, DISPATCHER_TIMER_LOG);
}

TimerDispatcher::~TimerDispatcher()
{

}

int TimerDispatcher::DISPATCHER_TIMER_LOG(void* object)
{
	LOG_DEBUG(L"log");

	return 0;
}

