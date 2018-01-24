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

	// DB처리를 담당할 DB 스레드 생성
	dbThread = new DBThread(*this);

	// Redis처리를 담당할 Redis 스레드 생성
	redisThread = new RedisThread(*this);

	// IO를 담당할 네트워크 스레드 생성 (Worker Thread)
#ifdef _WINDOWS
	networkThread = new NetworkThread[4];
	for (int i = 0; i < 4; ++i)
	{
		networkThread[i].SetNetEngine(this);
	}
#else //_WINDOWS
	networkThread = new NetworkThread(this);
#endif //_WINDOWS

	// 패킷을 처리할 메인스레드(Logic Thread) 생성
	mainThread = new MainThread(*this);

	// IO객체 생성
	ioMultiplexer = new IOMultiplexer();
	if (ioMultiplexer->create() == false)
		return false;

	// DB연결을 관리하는 매니저 생성
	dbManager = new DBManager;

	// Redis연결을 관리하는 매니저 생성
	redisManager = new RedisManager;
	
	// 연결된 세션들을 관리하기 위한 세션 매니저 생성
	sessionManager = new SessionManager(*this);

	// 여러개의 port를 열기위한 accept 매니저 생성 (일반 서버는 1개만 사용)
	acceptorManager = new AcceptorManager(ioMultiplexer, sessionManager);

	// 패킷을 담아놓을 큐 매니저 생성
	messageQueue = new MessageQueue();

	// 타이머를 담아놓은 큐 생성
	timerQueue = new TimerQueue();

	// 초기화 완료
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
	// 동작플래그를 켜고, 필요한 스레드를 동작시킨다
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

	// 동작플래그 OFF
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


