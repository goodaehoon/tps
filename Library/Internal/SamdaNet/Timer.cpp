#include "Precompiled.h"
#include "Timer.h"

Timer::Timer()
	: timer_id(0)
	, interval(0)
	, object(nullptr)
	, repeat(false)
	, active_time(0)
	, enable(false)
{
}

Timer::Timer(unsigned int id, unsigned int interval, void* object, bool repeat)
	: timer_id(id)
	, interval(interval)
	, object(object)
	, repeat(repeat)
	, active_time(0)
	, enable(false)
{
}

bool Timer::operator < (const Timer& rhs) const
{
	return active_time < rhs.active_time;
}