#pragma once

class Timer
{
public:
	unsigned int timer_id;		// 타이머 ID (디스패치용)
	void* object;				// 타이머 내용을 처리하는데 필요한 대상 오브젝트를 넘겨준다. dispatcher에서 캐스팅하여 사용
	unsigned int interval;		// 타이머가 동작하는 주기
	bool repeat;				// 반복여부 (true면 계속반복, false면 한번 실행되고 삭제)
	bool enable;				// 타이머가 유효한가? true이면 사용, false이면 Q에서 처리되지 않고 버린다
	time_t active_time;			// 타이머가 작동 할 시간 (Q에 들어갈 때 현재시간에 interval을 더한 값으로 자동으로 설정)

public:
	Timer();
	Timer(unsigned int id, unsigned int interval, void* object, bool repeat);

	bool operator < (const Timer& rhs) const;
};

#define TIMER_ONCE(id, interval, object)			\
	Timer timer_##id(id, interval, object, false);	\
	TIMER_QUEUE()->Push(timer_##id);

#define TIMER_REPEAT(id, interval, object)			\
	Timer timer_##id(id, interval, object, true);	\
	TIMER_QUEUE()->Push(timer_##id);
	