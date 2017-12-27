#pragma once

class User;
class Session;

class UserManager
{
private:
	unordered_map< unsigned int, User* > userManager;

public:
	UserManager();
	~UserManager();

	User* CreateUser(Session* s);
	void AddUser(User* user);
	void RemoveUser(User* user);
	User* FindUser(unsigned int userid);
};
