#include "Precompiled.h"
#include "TimeUtil.h"
#include <time.h>

#ifdef _WINDOWS
struct tm* localtime_r(const time_t* t, struct tm* r)
{
	struct tm* p = localtime(t);
	if (p)
		*r = *p;
	else
		memset(r, 0, sizeof(struct tm));

	return r;
}
#endif

time_t TimeUtil::CurTime()
{
	time_t now = time(0);
	return now;
}

void TimeUtil::CurTimeTM(struct tm* result)
{
	time_t curr_time = CurTime();
	localtime_r(&curr_time, result);
}
