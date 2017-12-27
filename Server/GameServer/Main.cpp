#include "Precompiled.h"
#include "SamdaNet.h"
#include "ClientDispatcher.h"
#include "Log.h"
#include <mysql.h>
#include "MySQLConnector.h"
#include "StringUtil.h"
#include "DBDispatcher.h"
#include "Application.h"
#include "Log.h"

int wmain(int argc, wchar_t* argv[])
{
	Application& app = Application::GetInstance();

	bool ret = app.Init(argc, argv);
	if (ret == false)
	{
		LOG_CRI(L"Application fail to init");
		return 1;
	}

	app.Process();

	app.Close();

	return 0;
}

