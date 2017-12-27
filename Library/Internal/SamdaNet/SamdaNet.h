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
	// IOó�� ��ü
	IOMultiplexer* ioMultiplexer;
	// ����� ������ ������ ��ü
	SessionManager*	sessionManager;
	// listen�� port�� ������ ��ü
	AcceptorManager* acceptorManager;
	// connect�� port�� ������ ��ü
	ConnectorManager* connectorManager;
	// DB������ ������ ��ü
	DBManager* dbManager;
	// Redis������ ������ ��ü
	RedisManager* redisManager;
	// IO�� ����� ������
	NetworkThread* networkThread;
	// ��Ŷó�� (����Ͻ�����)�� ����� ������
	MainThread* mainThread;
	// DBó���� ����� ������
	DBThread* dbThread;
	// Redisó���� ����� ������
	RedisThread* redisThread;
	// ��Ŷ(��Ʈ��)�� ��Ƴ��� ť �Ŵ���
	MessageQueue* messageQueue;
	// Ÿ�̸Ӹ� ��Ƴ��� ť
	TimerQueue* timerQueue;
	// ����� �غ�(�ʱ�ȭ)�� �Ǿ��°�?
	bool initialized;
	// Ȱ��ȭ �����ΰ�?
	bool running;
	// Ȱ������ ����ȭ�� ���� ������Ʈ
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

