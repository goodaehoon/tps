#pragma once

class Redis;

class RedisManager
{
private:
	unordered_map<int, Redis* > redisHandles;	// < Redis Handle Index, Redis * >

public:
	RedisManager();
	virtual ~RedisManager();

	bool Open(int idx, const wchar_t* ip, unsigned int port);
	void Close();

	Redis* GetRedisHandle(int idx);

};
