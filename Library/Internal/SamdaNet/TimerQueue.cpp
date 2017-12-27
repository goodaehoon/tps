#include "Precompiled.h"
#include "TimerQueue.h"
#include "TimeUtil.h"
#include "Timer.h"

void TimerQueue::Push(Timer& timer)
{
	LockGuard<Mutex> lock(syncObj);

	timer.active_time = TimeUtil::CurTime() + timer.interval;
	timer.enable = true;

	timerQueue.push(timer);
}

bool TimerQueue::Pop(Timer& timer)
{
	LockGuard<Mutex> lock(syncObj);

	if (timerQueue.empty() == true)
		return false;

	timer = timerQueue.top();
	if (timer.active_time <= TimeUtil::CurTime())
	{
		timerQueue.pop();
		return true;
	}

	return false;
}

size_t TimerQueue::Size()
{
	LockGuard<Mutex> lock(syncObj);

	return timerQueue.size();
}

bool TimerQueue::Empty()
{
	LockGuard<Mutex> lock(syncObj);

	return timerQueue.empty();
}