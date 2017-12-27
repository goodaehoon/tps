#pragma once

#include <hiredis.h>

class Redis
{
private:

	redisContext* ctx;					// 레디스 핸들
	redisReply* reply;					// 레디스 결과 응답

public:
	Redis();
	~Redis();

	bool Open(const wchar_t* ip, unsigned int port);
	void Close();
};