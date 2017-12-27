#pragma once

#include "Thread.h"
#include "Socket.h"

class SamdaNet;

// ������������ ����ϵ��� �Ѵ�
class AcceptThread : public Thread
{
private:
	SamdaNet& _samdanet;

	Socket _sock;

	bool _open;	

public:
	AcceptThread(SamdaNet& samdanet);

#ifdef _WINDOWS
#else //_WINDOWS
	void* process();

	bool open(unsigned short port);

	bool open(const char* ip, unsigned short port);
#endif //_WINDOWS
};

