#pragma once

#pragma pack(push, 1)

#include "Session.h"

struct InterMsgHeader
{
	unsigned int msgSize;
	unsigned short msgNo;
	unsigned int sessionID;
	Session* sessionPtr;

	InterMsgHeader()
		: msgSize(0)
		, msgNo(0)
		, sessionID(0)
		, sessionPtr(nullptr)
	{}

	InterMsgHeader(unsigned int msg_size, unsigned short msg_no, Session* session_ptr)
		: msgSize(msg_size)
		, msgNo(msg_no)
		, sessionID(session_ptr->GetID())
		, sessionPtr(session_ptr)
	{}

	unsigned short GetMsgNo() { return msgNo; }
	unsigned int GetMsgSize() { return msgSize; }
	unsigned int GetSessionID() { return sessionID; }
	Session* GetSessionPtr() { return sessionPtr; }
};

#pragma pack(pop)
