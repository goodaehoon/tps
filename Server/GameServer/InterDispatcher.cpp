#include "Precompiled.h"
#include "InterDispatcher.h"
#include "Stream.h"
#include "InterPacket.h"
#include "MySQLConnector.h"
#include "DBManager.h"
#include "SamdaNet.h"
#include "StringUtil.h"
#include "MessageQueue.h"
#include "Application.h"
#include "ByteBuffer.h"
#include "GamePacket.h"

using namespace GamePacket;
using namespace InterPacket;

InterDispatcher::InterDispatcher()
{
	RegisterDispatcher(InterMsgNo::INTER_LOGIN_RES, DISPATCHER_INTER_LOGIN_RESPONSE);
}

InterDispatcher::~InterDispatcher()
{

}

int InterDispatcher::DISPATCHER_INTER_LOGIN_RESPONSE(Session* session, InterMsg* msg)
{
	INTER_LOGIN_RES* login_res = reinterpret_cast<INTER_LOGIN_RES *>(msg->GetReadBuffer());

	LOGIN_RESPONSE sc_login;
	sc_login.result = login_res->result;
	sc_login.m_id = login_res->m_id;
	session->Send(&sc_login);

	return 0;
}

