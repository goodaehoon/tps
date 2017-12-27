#pragma once

#pragma pack(push, 1)

struct PacketHeader
{
	unsigned short packetVerifier;		// 항상 0xCFDA 값입니다. 패킷이 유효한지와 패킷간 구별을 위한 용도입니다.
	unsigned int packetSize;			// 패킷 헤더를 포함한 전체 패킷 크기
	unsigned short packetNo;			// 패킷 번호

	PacketHeader()
		: packetVerifier(0xCFDA)
		, packetSize(0)
		, packetNo(0)
	{}

	PacketHeader(unsigned int packet_size, unsigned short packet_id)
		: packetVerifier(0xCFDA)
		, packetSize(packet_size)
		, packetNo(packet_id)
	{}

	unsigned short GetPacketVerifier() { return packetVerifier; }
	int GetPacketSize() { return packetSize; }
	unsigned short GetPacketNo() { return packetNo; }
};

#pragma pack(pop)
