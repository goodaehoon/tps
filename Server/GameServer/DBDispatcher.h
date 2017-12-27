#pragma once

#include "Dispatcher.h"

class DBDispatcher : public IDBDispatcher
{
public:
	DBDispatcher();
	virtual ~DBDispatcher() override;

	static int DISPATCHER_DB_LOGIN_REQ(InterMsg* msg);
};