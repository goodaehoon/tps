#pragma once

#include "InterMsgHeader.h"

#pragma pack(push, 1)

class Session;

namespace InterPacket
{
	namespace InterMsgNo
	{
		enum ePktID
		{
			DB_LOGIN_REQ,
			REDIS_LOGIN_REQ,
			INTER_LOGIN_RES,
			MAX,
		};
	}

	struct DB_LOGIN_REQ : public InterMsgHeader
	{
		DB_LOGIN_REQ(Session* session)
			: InterMsgHeader(sizeof(DB_LOGIN_REQ), InterMsgNo::DB_LOGIN_REQ, session)
		{
			memset(userName, 0, sizeof(userName));
		}

		char userName[32];
	};

	struct REDIS_LOGIN_REQ : public InterMsgHeader
	{
		REDIS_LOGIN_REQ(Session* session)
			: InterMsgHeader(sizeof(REDIS_LOGIN_REQ), InterMsgNo::REDIS_LOGIN_REQ, session)
		{
			memset(userName, 0, sizeof(userName));
		}

		char userName[32];
	};

	struct INTER_LOGIN_RES : public InterMsgHeader
	{
		INTER_LOGIN_RES(Session* session)
			: InterMsgHeader(sizeof(INTER_LOGIN_RES), InterMsgNo::INTER_LOGIN_RES, session)
			, result(0)
			, m_id(0)
		{
		}

		int result;
		int m_id;
	};
}

#pragma pack(pop)
