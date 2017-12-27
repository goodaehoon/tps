#pragma once

class User;
class UserManager;

class GameManager
{
	UserManager* userManager;

public:
	GameManager();
	~GameManager();

	bool Init();
	UserManager* GetUserManager();
};