#include "Precompiled.h"
#include "Thread.h"
#include "Log.h"

Thread::Thread()
	: threadID(0)
#ifdef _WINDOWS
	, threadHandle(INVALID_HANDLE_VALUE)
#endif //_WINDOWS
{
}

Thread::~Thread()
{
	threadID = 0;
#ifdef _WINDOWS
	CloseHandle(threadHandle);
	threadHandle = INVALID_HANDLE_VALUE;
#endif //_WINDOWS
}

#ifdef _WINDOWS
unsigned int WINAPI Thread::start_function(void* arg)
#else //_WINDOWS
void* Thread::start_function(void* arg)
#endif //_WINDOWS
{
	Thread* inst = (Thread*)arg;
	inst->Process();
	return 0;
}

bool Thread::Start()
{
#ifdef _WINDOWS
	threadHandle = (HANDLE)_beginthreadex(nullptr, 0, Thread::start_function, this, 0, (unsigned int *)&threadID);
	if (threadHandle == 0)
	{
		LOG_CORE(L"fail _beginthreadex() error(%d)\n", errno);
		return false;
	}
#else //_WINDOWS
	int retval = pthread_create(&threadID, nullptr, Thread::start_routine, this);
	if (retval != 0)
	{
		LOG_CORE(L"fail pthread_create() error(%d): %s\n", retval, strerror(retval));
		return false;
	}
#endif //_WINDOWS
	return true;
}

void Thread::Stop()
{
#ifdef _WINDOWS
	if (WaitForSingleObject(threadHandle, INFINITE) == WAIT_FAILED)
		LOG_CORE(L"fail WaitForSingleObject() error(%d)\n", GetLastError());
#else //_WINDOWS
	int retval = pthread_join(threadID, nullptr);
	if (retval != 0)
		LOG_CORE(L"fail pthread_join() error(%d): %s\n", retval, strerror(retval));
#endif //_WINDOWS
}


#ifdef _WINDOWS
unsigned int Thread::GetID()
#else //_WINDOWS
pthread_t Thread::GetID()
#endif //_WINDOWS
{
	return threadID;
}