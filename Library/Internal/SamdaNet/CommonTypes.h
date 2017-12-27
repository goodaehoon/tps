#pragma once

#define SAFE_DELETE(p) 	if (p) { delete   p; p = nullptr; }
#define SAFE_DELETES(p) 	if (p) { delete[] p; p = nullptr; }

#define MAX_SESSION_COUNT 1024

#define MAX_SEND_BUFFER 4096
#define MAX_RECV_BUFFER 4096

#define PACKET_POOL_SIZE 4096
#define PACKET_POOL_INIT_COUNT 1024
#define INTERMSG_POOL_SIZE 4096
#define INTERMSG_POOL_INIT_COUNT 1024

enum EventErrorType
{
	EVENT_ERROR_IO = -1,
	EVENT_ERROR_PARSING = -2,
};

#ifdef _WINDOWS
typedef int socklen_t;
#else
typedef int SOCKET;
#define INVALID_SOCKET -1
#endif