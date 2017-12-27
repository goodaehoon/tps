#include "Precompiled.h"
#include "User.h"
#include "Session.h"

User::User(Session* s)
	: session(s)
	, userID(0)
{
	memset(userName, 0, sizeof(userName));
}

User::~User()
{
	Clear();
}

void User::Clear()
{
	session = nullptr;
	userID = 0;
	memset(userName, 0, sizeof(userName));
}

Session* User::GetSession()
{
	return session;
}

unsigned int User::GetSessionID()
{
	return session->GetID();
}

unsigned int User::GetUserID()
{
	return userID;
}

wchar_t* User::GetUserName()
{
	return userName;
}
