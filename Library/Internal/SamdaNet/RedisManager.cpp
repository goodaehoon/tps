#include "Precompiled.h"
#include "RedisManager.h"
#include "Redis.h"


RedisManager::RedisManager()
{

}

RedisManager::~RedisManager()
{
	Close();
}

bool RedisManager::Open(int idx, const wchar_t* ip, unsigned int port)
{
	if (GetRedisHandle(idx) != nullptr)
	{
		return false;
	}

	Redis* redis = new Redis;
	if (!redis->Open(ip, port))
	{
		return false;
	}

	redisHandles.insert(make_pair(idx, redis));

	return true;
}

void RedisManager::Close()
{
	for (auto itr : redisHandles)
	{
		Redis* redis = itr.second;

		redis->Close();

		SAFE_DELETE(redis);
	}
}

Redis* RedisManager::GetRedisHandle(int idx)
{
	auto itr = redisHandles.find(idx);
	if (itr == redisHandles.end())
		return nullptr;

	return redisHandles[idx];
}
