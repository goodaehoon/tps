#pragma once

#include "GlobalDefines.h"

struct NetworkConfig
{
	wstring ip;
	unsigned short port;

	NetworkConfig()
		: ip(L""), port(0)
	{};
};

struct LogConfig
{
	wstring logPath;

	LogConfig()
		: logPath(L"")
	{}
};

struct MysqlConfig
{
	wstring ip;
	unsigned short port;
	wstring user;
	wstring passwd;
	wstring dbname;

	MysqlConfig()
		: ip(L""), port(0), user(L""), passwd(L""), dbname(L"")
	{};
};

struct RedisConfig
{
	wstring ip;
	unsigned short port;

	RedisConfig()
		: ip(L""), port(0)
	{};
};

class ConfigManager
{
public:
	ConfigManager();

	NetworkConfig networkConfig;
	LogConfig logConfig;
	MysqlConfig mysqlConfig[MAX_DB_HANDLE];
	RedisConfig redisConfig[MAX_REDIS_HANDLE];

	bool ReadConfig(wchar_t* fullpath);
};
