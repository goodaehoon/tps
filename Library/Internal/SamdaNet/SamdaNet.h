#pragma once

/*
 ____                      _       _   _      _
/ ___|  __ _ _ __ ___   __| | __ _| \ | | ___| |_
\___ \ / _` | '_ ` _ \ / _` |/ _` |  \| |/ _ \ __|
___) | (_| | | | | | | (_| | (_| | |\  |  __/ |_
|____/ \__,_|_| |_| |_|\__,_|\__,_|_| \_|\___|\__| by figlet
	
	Since 2016.11 ~
*/

#include "Mutex.h"

class IOMultiplexer;
class SessionManager;
class AcceptorManager;
class ConnectorManager;
class DBManager;
class RedisManager;
class NetworkThread;
class MainThread;
class DBThread;
class RedisThread;
class IDispatcher;
class IDBDispatcher;
class IInterDispatcher;
class ITimerDispatcher;
class IRedisDispatcher;
class EventObject;
class MessageQueue;
class TimerQueue;
class Acceptor;
class Connector;
class Session;

class SamdaNet 
{
private:
	// IO처리 객체
	IOMultiplexer* ioMultiplexer;
	// 연결된 세션을 관리할 객체
	SessionManager*	sessionManager;
	// listen할 port를 관리할 객체
	AcceptorManager* acceptorManager;
	// connect한 port를 관리할 객체
	ConnectorManager* connectorManager;
	// DB연결을 관리할 객체
	DBManager* dbManager;
	// Redis연결을 관리할 객체
	RedisManager* redisManager;
	// IO를 담당할 스레드
	NetworkThread* networkThread;
	// 패킷처리 (비즈니스로직)를 담당할 스레드
	MainThread* mainThread;
	// DB처리를 담당할 스레드
	DBThread* dbThread;
	// Redis처리를 담당할 스레드
	RedisThread* redisThread;
	// 패킷(스트림)을 담아놓은 큐 매니저
	MessageQueue* messageQueue;
	// 타이머를 담아놓은 큐
	TimerQueue* timerQueue;
	// 사용할 준비(초기화)가 되었는가?
	bool initialized;
	// 활성화 상태인가?
	bool running;
	// 활성상태 동기화를 위한 오브젝트
	Mutex runningSync;

public:
	SamdaNet();
	~SamdaNet();

	bool Start();
	void Stop();
	void SetRunning(bool flag);
	bool IsRunning();	
	bool Open(const wchar_t* ip, unsigned short port, IDispatcher* disp);
	bool Connect(const wchar_t* ip, unsigned short port, IDispatcher* disp);
	void SetDBDispatcher(IDBDispatcher* disp);
	void SetInterDispatcher(IInterDispatcher* disp);
	void SetTimerDispatcher(ITimerDispatcher* disp);
	void SetRedisDispatcher(IRedisDispatcher* disp);

	SessionManager* GetSessionManager();
	IOMultiplexer* GetIOMultiplexer();
	MessageQueue* GetMessageQueue();
	TimerQueue* GetTimerQueue();
	DBManager* GetDBManager();
	RedisManager* GetRedisManager();

private:
	bool Init();
	void Close();	
};

