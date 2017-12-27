#include "Precompiled.h"
#include "UserManager.h"
#include "User.h"
#include "Session.h"

UserManager::UserManager()
{

}

UserManager::~UserManager()
{
	for (auto itr : userManager)
	{
		delete itr.second;
	}
}

User* UserManager::CreateUser(Session* s)
{
	User* user = new User(s);	

	return user;
}

void UserManager::AddUser(User* user)
{
	userManager.insert(make_pair(user->GetSessionID(), user));
}

void UserManager::RemoveUser(User* user)
{
	if (user)
	{
		userManager.erase(user->GetSessionID());
	}
}

User* UserManager::FindUser(unsigned int session_id)
{
	auto itr = userManager.find(session_id);
	if (itr == userManager.end())
		return nullptr;

	return itr->second;
}
