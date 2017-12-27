#pragma once

#include "Thread.h"
#include "Socket.h"

class SamdaNet;

// 리눅스에서만 사용하도록 한다
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

