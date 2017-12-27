#include "Precompiled.h"
#include "RedisDispatcher.h"
#include "Stream.h"
#include "InterPacket.h"
#include "Redis.h"
#include "RedisManager.h"
#include "SamdaNet.h"
#include "StringUtil.h"
#include "MessageQueue.h"
#include "Application.h"
#include "ByteBuffer.h"

using namespace InterPacket;

RedisDispatcher::RedisDispatcher()
{
	RegisterDispatcher(InterMsgNo::REDIS_LOGIN_REQ, DISPATCHER_REDIS_LOGIN_REQ);
}

RedisDispatcher::~RedisDispatcher()
{

}

int RedisDispatcher::DISPATCHER_REDIS_LOGIN_REQ(InterMsg* msg)
{
	REDIS_LOGIN_REQ* req = reinterpret_cast<REDIS_LOGIN_REQ*>(msg->GetReadBuffer());

	Redis* redis = REDIS_MANAGER()->GetRedisHandle(0);

	INTER_LOGIN_RES login_res(req->GetSessionPtr());
	login_res.result = 1;
	login_res.m_id = 0;

	MESSAGE_QUEUE()->Push(QueueType::INTER_MSG_Q, &login_res);

	return 0;
}

