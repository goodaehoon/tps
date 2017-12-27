#pragma once

class MySQL;

class DBManager
{
private:
	unordered_map<int, MySQL* > dbHandles;	// < DB Handle Index, MySQL * >

public:
	DBManager();
	virtual ~DBManager();

	bool Open(int idx, const wchar_t* ip, unsigned int port, const wchar_t* dbname, const wchar_t* id, const wchar_t* passwd);
	void Close();

	MySQL* GetDBHandle(int idx);

};
