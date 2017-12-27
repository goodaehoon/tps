#include "Precompiled.h"
#include "Mutex.h"

Mutex::Mutex()
{
#ifdef _WINDOWS
	InitializeCriticalSection(&_lockobj);
#else
	pthread_mutex_init(&_lockobj, 0);
#endif
}

Mutex::~Mutex()
{
#ifdef _WINDOWS
	DeleteCriticalSection(&_lockobj);
#else
	pthread_mutex_destroy(&_lockobj);
#endif
}

void Mutex::Lock()
{
#ifdef _WINDOWS
	EnterCriticalSection(&_lockobj);
#else
	pthread_mutex_lock(&_lockobj);
#endif
}

void Mutex::Unlock()
{
#ifdef _WINDOWS
	LeaveCriticalSection(&_lockobj);
#else
	pthread_mutex_unlock(&_lockobj);
#endif
}
