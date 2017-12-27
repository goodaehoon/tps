#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifdef _WINDOWS
#include <WinSock2.h>
#include <Mswsock.h>
#include <io.h>
#else //_WINDOWS
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#endif //_WINDOWS

#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

using namespace std;
