#pragma once

#include "GlobalDefines.h"

class Session;

class User
{
private:
	Session* session;
	unsigned int userID;
	wchar_t userName[MAX_USER_NAME];

public:
	User(Session* s);
	~User();

	void Clear();

	Session* GetSession();
	unsigned int GetSessionID();
	unsigned int GetUserID();
	wchar_t* GetUserName();
};
