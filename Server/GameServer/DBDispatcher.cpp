#include "Precompiled.h"
#include "DBDispatcher.h"
#include "Stream.h"
#include "InterPacket.h"
#include "MySQLConnector.h"
#include "DBManager.h"
#include "SamdaNet.h"
#include "StringUtil.h"
#include "MessageQueue.h"
#include "Application.h"
#include "ByteBuffer.h"

using namespace InterPacket;

DBDispatcher::DBDispatcher()
{
	RegisterDispatcher(InterMsgNo::DB_LOGIN_REQ, DISPATCHER_DB_LOGIN_REQ);
}

DBDispatcher::~DBDispatcher()
{

}

int DBDispatcher::DISPATCHER_DB_LOGIN_REQ(InterMsg* msg)
{
	DB_LOGIN_REQ* req = reinterpret_cast<DB_LOGIN_REQ*>(msg->GetReadBuffer());

	MySQL* db = DB_MANAGER()->GetDBHandle(ACCOUNT_DB);
	db->ExecuteQuery(L"select m_id from member where user_id = '%s'", MbcsToWcs(req->userName));
	db->Fetch();
	unsigned int m_id = 0;
	db->GetInt(m_id);
	wprintf(L"m_id = %d\n", m_id);

	INTER_LOGIN_RES login_res(req->GetSessionPtr());
	login_res.result = 1;
	login_res.m_id = m_id;

	MESSAGE_QUEUE()->Push(QueueType::INTER_MSG_Q, &login_res);

	return 0;
}

