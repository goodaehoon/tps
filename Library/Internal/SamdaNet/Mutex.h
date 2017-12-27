#pragma once

class Mutex
{
private:
#ifdef _WINDOWS
	CRITICAL_SECTION _lockobj;
#else
	pthread_mutex_t _lockobj;
#endif

public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();
};

template <typename T>
class LockGuard
{
private:
	T& _mutex;

public:
	LockGuard(T& mutex)
		: _mutex(mutex)
	{
		_mutex.Lock();
	}

	~LockGuard()
	{
		_mutex.Unlock();
	}
};