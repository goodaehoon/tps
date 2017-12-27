#include "Precompiled.h"
#include "DBManager.h"
#include "MySQLConnector.h"


DBManager::DBManager()
{

}

DBManager::~DBManager()
{
	Close();
}

bool DBManager::Open(int idx, const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd)
{
	if (GetDBHandle(idx) != nullptr)
	{
		return false;
	}

	MySQL* db = new MySQL;
	if (!db->Open(ip, port, dbname, id, passwd))
	{
		return false;
	}

	dbHandles.insert(make_pair(idx, db));

	return true;
}

void DBManager::Close()
{
	for (auto itr : dbHandles)
	{
		MySQL* db = itr.second;

		db->Close();

		SAFE_DELETE(db);
	}
}

MySQL* DBManager::GetDBHandle(int idx)
{
	auto itr = dbHandles.find(idx);
	if (itr == dbHandles.end())
		return nullptr;

	return dbHandles[idx];
}
