#include "Precompiled.h"
#include "ConfigManager.h"
#include "ConfigReader.h"
#include "Application.h"

ConfigManager::ConfigManager()
{
}

bool ConfigManager::ReadConfig(wchar_t* fullpath)
{
	ConfigReader serverConf;
	serverConf.Read(fullpath);

	// 기본서버 설정을 읽는다
	networkConfig.ip = serverConf.Get<wstring>(L"NETWORK.ip", L"127.0.0.1");
	networkConfig.port = serverConf.Get<int>(L"NETWORK.port", 5001);

	// 로그 설정을 읽는다
	logConfig.logPath = serverConf.Get<wstring>(L"LOG.log_path", L"./log");

	// Mysql DB 설정을 읽는다
	mysqlConfig[ACCOUNT_DB].ip = serverConf.Get<wstring>(L"MYSQL.account_db_ip", L"52.79.154.180");
	mysqlConfig[ACCOUNT_DB].port = serverConf.Get<int>(L"MYSQL.account_db_port", 3306);
	mysqlConfig[ACCOUNT_DB].user = serverConf.Get<wstring>(L"MYSQL.account_db_user", L"admin");
	mysqlConfig[ACCOUNT_DB].passwd = serverConf.Get<wstring>(L"MYSQL.account_db_passwd", L"tjqjxla!1");
	mysqlConfig[ACCOUNT_DB].dbname = serverConf.Get<wstring>(L"MYSQL.account_db_name", L"account");

	mysqlConfig[GAME_DB].ip = serverConf.Get<wstring>(L"MYSQL.game_db_ip", L"52.79.154.180");
	mysqlConfig[GAME_DB].port = serverConf.Get<int>(L"MYSQL.game_db_port", 3306);
	mysqlConfig[GAME_DB].user = serverConf.Get<wstring>(L"MYSQL.game_db_user", L"admin");
	mysqlConfig[GAME_DB].passwd = serverConf.Get<wstring>(L"MYSQL.game_db_passwd", L"tjqjxla!1");
	mysqlConfig[GAME_DB].dbname = serverConf.Get<wstring>(L"MYSQL.game_db_name", L"account");

	// Redis 설정을 읽는다
	redisConfig[MASTER_REDIS].ip = serverConf.Get<wstring>(L"master_redis_ip", L"52.79.155.216");
	redisConfig[MASTER_REDIS].port = serverConf.Get<int>(L"master_redis_port", 6379);

	return true;
}
