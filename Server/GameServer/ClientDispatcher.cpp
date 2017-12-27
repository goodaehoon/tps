#include "Precompiled.h"
#include "ByteBuffer.h"
#include "ClientDispatcher.h"
#include "Stream.h"
#include "GamePacket.h"
#include "Log.h"
#include "SamdaNet.h"
#include "Application.h"
#include "InterPacket.h"
#include "GameManager.h"
#include "User.h"
#include "UserManager.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "TimeUtil.h"

using namespace GamePacket;
using namespace InterPacket;

ClientDispatcher::ClientDispatcher()
{
	RegisterDispatcher(PacketNo::LOGIN_REQUEST, DISPATCHER_LOGIN_REQUEST);
}

ClientDispatcher::~ClientDispatcher()
{
	
}

void ClientDispatcher::OnConnected(Session* session)
{
	LOG_INFO(L"OnConnected!");
}

void ClientDispatcher::OnDisconnected(Session* session, int err)
{
	USER_MANAGER()->RemoveUser(USER_MANAGER()->FindUser(session->GetID()));

	LOG_INFO(L"OnDisconnected(%d)", err);
}

int ClientDispatcher::DISPATCHER_LOGIN_REQUEST(Session* session, Packet* packet)
{
	char buffer[256] = { 0, };
	int strlen = 0;	

	PacketHeader* packet_header = packet->GetHeader();
	strlen = packet->GetString(buffer);
	
	DB_LOGIN_REQ db_login_req(session);
	strncpy_s(db_login_req.userName, buffer, strlen);
	MESSAGE_QUEUE()->Push(DB_Q, &db_login_req);

	User* user = USER_MANAGER()->CreateUser(session);
	USER_MANAGER()->AddUser(user);

	return 0;
}

