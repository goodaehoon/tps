#pragma once

#ifdef _WINDOWS
struct tm* localtime_r(const time_t* t, struct tm* r);
#endif

class TimeUtil
{
public:
	static time_t CurTime();
	static void CurTimeTM(struct tm* result);
	
};
