#include "Precompiled.h"
#include "Redis.h"
#include "StringUtil.h"
#include "Log.h"

Redis::Redis()
	: ctx(nullptr)
	, reply(nullptr)
{
}

Redis::~Redis()
{
	Close();
}

bool Redis::Open(const wchar_t* ip, unsigned int port)
{
	struct timeval timeout = { 1, 500000 }; // 1.5 seconds
	ctx = redisConnectWithTimeout((const char*)WcsToMbcs(ip), port, timeout);
	if (ctx->err) 
	{
		return false;
	}

	return true;
}

void Redis::Close()
{
	redisFree(ctx);
	freeReplyObject(reply);

	reply = nullptr;
	ctx = nullptr;
}

