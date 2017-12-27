#pragma once

#include "Mutex.h"
#include "Timer.h"
#include <queue>

class TimerQueue
{
private:
	priority_queue<Timer> timerQueue;
	Mutex syncObj;

public:
	void Push(Timer& timer);
	bool Pop(Timer& timer);
	size_t Size();
	bool Empty();
};

