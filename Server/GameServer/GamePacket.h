#pragma once

#include "PacketHeader.h"

#pragma pack(push, 1)

namespace GamePacket
{
	namespace PacketNo
	{
		enum ePktID
		{
			RESULT_INFO,
			LOGIN_REQUEST,
			LOGIN_RESPONSE,			
			MAX,
		};
	}

	struct RESULT_INFO : public PacketHeader
	{
		RESULT_INFO()
			: PacketHeader(sizeof(RESULT_INFO), PacketNo::RESULT_INFO)
		{}

		enum eResultCode
		{
			SERVER_ERROR,				// 일반적인 오류
			WRONG_CHANNEL_PASSWORD,		// 잘못된 채널 입장 비밀번호.
			NOT_FOUND_CHANNEL,			// 존재 하지 않는 채널
		};

		unsigned int result;
	};

	struct LOGIN_REQUEST : public PacketHeader
	{
		LOGIN_REQUEST()
			: PacketHeader(sizeof(LOGIN_REQUEST), PacketNo::LOGIN_REQUEST)
		{
			memset(userName, 0, sizeof(userName));
		}

		char userName[32];
	};

	struct LOGIN_RESPONSE : public PacketHeader
	{
		LOGIN_RESPONSE()
			: PacketHeader(sizeof(LOGIN_RESPONSE), PacketNo::LOGIN_RESPONSE)
			, result(0)
			, m_id(0)
		{
		}

		int result;
		unsigned int m_id;
	};
}

#pragma pack(pop)
