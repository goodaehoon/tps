#include "Precompiled.h"
#include "GameManager.h"
#include "User.h"
#include "UserManager.h"

GameManager::GameManager()
	: userManager(nullptr)
{
}

GameManager::~GameManager()
{
	delete userManager;
}

bool GameManager::Init()
{
	userManager = new UserManager;

	return true;
}

UserManager* GameManager::GetUserManager()
{
	return userManager;
}
