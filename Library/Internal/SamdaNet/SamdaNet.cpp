#include "Precompiled.h"
#include "SamdaNet.h"
#include "Dispatcher.h"
#include "Socket.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"
#include "DBManager.h"
#include "RedisManager.h"
#include "NetworkThread.h"
#include "MainThread.h"
#include "DBThread.h"
#include "RedisThread.h"
#include "Dispatcher.h"
#include "TimerQueue.h"
#include "Acceptor.h"
#include "AcceptorManager.h"
#include "Connector.h"
#include "ConnectorManager.h"

SamdaNet::SamdaNet()
	: ioMultiplexer(nullptr)
	, sessionManager(nullptr)
	, dbManager(nullptr)
	, redisManager(nullptr)
	, networkThread(nullptr)
	, mainThread(nullptr)
	, dbThread(nullptr)
	, redisThread(nullptr)
	, acceptorManager(nullptr)
	, messageQueue(nullptr)
	, timerQueue(nullptr)
	, initialized(false) 
	, running(false)
{
	Init();
}

SamdaNet::~SamdaNet()
{
	Stop();

	Close();
}

bool SamdaNet::Init()
{
	if (initialized == true)
		return false;

#ifdef _WINDOWS
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 0), &WSAData) != 0)
		return false;
#endif 

	// DBó���� ����� DB ������ ����
	dbThread = new DBThread(*this);

	// Redisó���� ����� Redis ������ ����
	redisThread = new RedisThread(*this);

	// IO�� ����� ��Ʈ��ũ ������ ���� (Worker Thread)
#ifdef _WINDOWS
	networkThread = new NetworkThread[4];
	for (int i = 0; i < 4; ++i)
	{
		networkThread[i].SetNetEngine(this);
	}
#else //_WINDOWS
	networkThread = new NetworkThread(this);
#endif //_WINDOWS

	// ��Ŷ�� ó���� ���ν�����(Logic Thread) ����
	mainThread = new MainThread(*this);

	// IO��ü ����
	ioMultiplexer = new IOMultiplexer();
	if (ioMultiplexer->create() == false)
		return false;

	// DB������ �����ϴ� �Ŵ��� ����
	dbManager = new DBManager;

	// Redis������ �����ϴ� �Ŵ��� ����
	redisManager = new RedisManager;
	
	// ����� ���ǵ��� �����ϱ� ���� ���� �Ŵ��� ����
	sessionManager = new SessionManager(*this);

	// �������� port�� �������� accept �Ŵ��� ���� (�Ϲ� ������ 1���� ���)
	acceptorManager = new AcceptorManager(ioMultiplexer, sessionManager);

	// ��Ŷ�� ��Ƴ��� ť �Ŵ��� ����
	messageQueue = new MessageQueue();

	// Ÿ�̸Ӹ� ��Ƴ��� ť ����
	timerQueue = new TimerQueue();

	// �ʱ�ȭ �Ϸ�
	initialized = true;

	return true;
}

void SamdaNet::Close()
{
	SAFE_DELETE(dbManager);
	SAFE_DELETE(redisManager);
	SAFE_DELETE(acceptorManager);
	SAFE_DELETE(dbThread);
	SAFE_DELETE(redisThread);
	SAFE_DELETE(mainThread);
#ifdef _WINDOWS
	SAFE_DELETES(networkThread);
#else //_WINDOWS
	SAFE_DELETE(networkThread);
#endif //_WINDOWS
	SAFE_DELETE(sessionManager);
	SAFE_DELETE(messageQueue);
	SAFE_DELETE(timerQueue);
	SAFE_DELETE(ioMultiplexer);

#ifdef _WINDOWS
	WSACleanup();
#endif
}

SessionManager* SamdaNet::GetSessionManager()
{
	return sessionManager;
}

MessageQueue* SamdaNet::GetMessageQueue()
{
	return messageQueue;
}

TimerQueue* SamdaNet::GetTimerQueue()
{
	return timerQueue;
}

IOMultiplexer* SamdaNet::GetIOMultiplexer()
{
	return ioMultiplexer;
}

DBManager* SamdaNet::GetDBManager()
{
	return dbManager;
}

RedisManager* SamdaNet::GetRedisManager()
{
	return redisManager;
}

bool SamdaNet::Start()
{
	// �����÷��׸� �Ѱ�, �ʿ��� �����带 ���۽�Ų��
	SetRunning(true);

	mainThread->Start();

	networkThread->Start();
	
	dbThread->Start();
	
	redisThread->Start();

	return true;
}

void SamdaNet::Stop()
{
	networkThread->Stop();

	mainThread->Stop();

	redisThread->Stop();

	dbThread->Stop();		

	// �����÷��� OFF
	SetRunning(false);
}

void SamdaNet::SetRunning(bool flag)
{
	LockGuard<Mutex> lock(runningSync);

	running = flag;
}

bool SamdaNet::IsRunning()
{
	LockGuard<Mutex> lock(runningSync);

	return running;
}

void SamdaNet::SetDBDispatcher(IDBDispatcher* disp)
{
	dbThread->SetDBDispatcher(disp);
}

void SamdaNet::SetInterDispatcher(IInterDispatcher* disp)
{
	mainThread->SetInterDispatcher(disp);
}

void SamdaNet::SetTimerDispatcher(ITimerDispatcher* disp)
{
	mainThread->SetTimerDispatcher(disp);
}

void SamdaNet::SetRedisDispatcher(IRedisDispatcher* disp)
{
	redisThread->SetRedisDispatcher(disp);
}

bool SamdaNet::Open(const wchar_t* ip, unsigned short port, IDispatcher* disp)
{
	Acceptor* acceptor = acceptorManager->create(disp);
	if (acceptor->Open(ip, port) == false)
		return false;

	return true;
}

bool SamdaNet::Connect(const wchar_t* ip, unsigned short port, IDispatcher* disp)
{
	Connector* connector = connectorManager->create(disp);
	if (connector->Connect(ip, port) == false)
		return false;

	return true;
}


