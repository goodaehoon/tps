#include "Precompiled.h"
#include "ByteBuffer.h"
#include "PacketHeader.h"
#include "InterMsgHeader.h"
#include "Stream.h"

Stream::Stream()
	: byteBuffer(nullptr)
	, callerID(0)
{
}

Stream::Stream(ByteBuffer* buffer)
	: byteBuffer(buffer)
	, callerID(0)
{
}

ByteBuffer* Stream::GetByteBuffer()
{
	return byteBuffer;
}

char* Stream::GetReadBuffer()
{
	return byteBuffer->ReadPtr();
}

char* Stream::GetWriteBuffer()
{
	return byteBuffer->WritePtr();
}

void Stream::Clear()
{

}

unsigned int Stream::GetCallerID()
{
	return callerID;
}


void Stream::SetCallerID(unsigned int id)
{
	callerID = id;
}

bool Stream::GetByte(char& o)
{
	o = (*(char *)byteBuffer->ReadPtr());
	byteBuffer->Read((int)sizeof(char));

	return true;
}

bool Stream::GetByte(unsigned char& o)
{
	o = (*(unsigned char *)byteBuffer->ReadPtr());
	byteBuffer->Read((int)sizeof(unsigned char));

	return true;
}

bool Stream::GetShort(short& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(short));
	byteBuffer->Read((int)sizeof(short));

	return true;
}

bool Stream::GetShort(unsigned short& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(unsigned short));
	byteBuffer->Read((int)sizeof(unsigned short));

	return true;
}

bool Stream::GetInt(int& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(int));
	byteBuffer->Read((int)sizeof(int));

	return true;
}

bool Stream::GetInt(unsigned int& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(unsigned int));
	byteBuffer->Read((int)sizeof(unsigned int));

	return true;
}

bool Stream::GetInt64(long long& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(long long));
	byteBuffer->Read((int)sizeof(long long));

	return true;
}

bool Stream::GetInt64(unsigned long long& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(unsigned long long));
	byteBuffer->Read((int)sizeof(unsigned long long));

	return true;
}

bool Stream::GetFloat(float& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(float));
	byteBuffer->Read((int)sizeof(float));

	return true;
}

bool Stream::GetDouble(double& o)
{
	memcpy((void *)&o, (void *)byteBuffer->ReadPtr(), sizeof(double));
	byteBuffer->Read((int)sizeof(double));

	return true;
}

bool Stream::GetBinary(char* out, int size)
{
	memcpy((void *)out, (void *)byteBuffer->ReadPtr(), (size_t)size);
	byteBuffer->Read(size);

	return true;
}

int Stream::GetString(char* out)
{
	int len = 0;
	GetInt(len);

	strncpy(out, byteBuffer->ReadPtr(), len);
	
	return len;
}

int Stream::GetString(wchar_t* out)
{
	int len = 0;
	GetInt(len);

	wcsncpy(out, (wchar_t *)byteBuffer->ReadPtr(), len);

	return len;
}

bool Stream::PutByte(char v)
{
	*byteBuffer->WritePtr() = v;
	byteBuffer->Write((int)sizeof(char));

	return true;
}

bool Stream::PutByte(unsigned char v)
{
	*byteBuffer->WritePtr() = v;
	byteBuffer->Write((int)sizeof(unsigned char));

	return true;
}

bool Stream::PutShort(short v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(short));
	byteBuffer->Write((int)sizeof(short));

	return true;
}

bool Stream::PutShort(unsigned short v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(unsigned short));
	byteBuffer->Write((int)sizeof(unsigned short));

	return true;
}

bool Stream::PutInt(int v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(int));
	byteBuffer->Write((int)sizeof(int));

	return true;
}

bool Stream::PutInt(unsigned int v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(unsigned int));
	byteBuffer->Write((int)sizeof(unsigned int));

	return true;
}

bool Stream::PutInt64(long long v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(long long));
	byteBuffer->Write((int)sizeof(long long));

	return true;
}

bool Stream::PutInt64(unsigned long long v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(unsigned long long));
	byteBuffer->Write((int)sizeof(unsigned long long));

	return true;
}

bool Stream::PutFloat(float v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(float));
	byteBuffer->Write((int)sizeof(float));

	return true;
}

bool Stream::PutDouble(double v)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)&v, sizeof(double));
	byteBuffer->Write((int)sizeof(double));

	return true;
}

bool Stream::PutBinary(char* v, int size)
{
	memcpy((void *)byteBuffer->WritePtr(), (void *)v, (size_t)size);
	byteBuffer->Write(size);

	return true;
}

bool Stream::PutString(char* v, int size)
{
	// put string length
	PutInt(size);

	// put string
	strncpy((char *)byteBuffer->WritePtr(), (char *)v, (size_t)size);
	byteBuffer->Write(size);

	return true;
}

bool Stream::PutString(wchar_t* v, int size)
{
	// put string length
	PutInt(size);

	// put string
	wcsncpy((wchar_t*)byteBuffer->WritePtr(), (wchar_t *)v, (size_t)size);
	byteBuffer->Write(size);

	return true;
}

Packet::Packet()
{
	byteBuffer = new ByteBuffer(PACKET_POOL_SIZE);
}

Packet::~Packet()
{
	delete byteBuffer;
}

PacketHeader* Packet::GetHeader()
{
	PacketHeader* header_ptr = reinterpret_cast<PacketHeader *>(byteBuffer->ReadPtr());
	byteBuffer->Read(sizeof(PacketHeader));

	return header_ptr;
}

PacketHeader* Packet::GetHeaderPtr()
{
	return reinterpret_cast<PacketHeader *>(byteBuffer->ReadPtr());
}

void Packet::MakePacket(unsigned short packetNo)
{
	PacketHeader header;
	header.packetSize = 0;
	header.packetNo = packetNo;

	PutInt(header.packetSize);
	PutShort(header.packetNo);
}

void Packet::EndPacket()
{
	PacketHeader* header_ptr = (PacketHeader*)byteBuffer->ReadPtr();
	header_ptr->packetSize = byteBuffer->Length();
}

InterMsg::InterMsg()
{
	byteBuffer = new ByteBuffer(INTERMSG_POOL_SIZE);
}

InterMsg::~InterMsg()
{
	delete byteBuffer;
}

InterMsgHeader* InterMsg::GetHeaderPtr()
{
	return reinterpret_cast<InterMsgHeader *>(byteBuffer->ReadPtr());
}

void InterMsg::MakeMessage(unsigned short msgid)
{
	InterMsgHeader header;
	header.msgSize = 0;
	header.msgNo = msgid;

	PutBinary((char*)&header, sizeof(header));
}

void InterMsg::EndMessage()
{
	InterMsgHeader* header_ptr = (InterMsgHeader*)byteBuffer->ReadPtr();
	header_ptr->msgSize = byteBuffer->Length();
}
