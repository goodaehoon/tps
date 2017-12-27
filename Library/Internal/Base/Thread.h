#pragma once

#ifdef _WINDOWS
#include <process.h>
#else //_WINDOWS
#include <pthread.h>
#endif //_WINDOWS

class Thread
{
protected:
#ifdef _WINDOWS
	HANDLE threadHandle;
	unsigned int threadID;
#else //_WINDOWS
	pthread_t threadID;
#endif //_WINDOWS

public:
	Thread();
	virtual ~Thread();
	
	bool Start();
	void Stop();

#ifdef _WINDOWS
	unsigned int GetID();
#else //_WINDOWS
	pthread_t GetID();
#endif //_WINDOWS


protected:

#ifdef _WINDOWS
	static unsigned int WINAPI start_function(void* arg);
#else //_WINDOWS
	static void* start_function(void* arg);
#endif //_WINDOWS

	virtual void* Process() = 0;
};
