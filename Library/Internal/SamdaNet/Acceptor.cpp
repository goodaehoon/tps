#include "Precompiled.h"
#include "Acceptor.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringUtil.h"

Acceptor::Acceptor(IOMultiplexer* ioMux, SessionManager* ssMgr, IDispatcher* disp)
	: ioMux(ioMux)
	, ssMgr(ssMgr)
	, dispatcher(disp)
#ifdef _WINDOWS
	, acceptOverlapped(this)
#endif
{
}

bool Acceptor::Open(const wchar_t* ip, unsigned short port)
{
	if (socket.Create(SOCK_STREAM, IPPROTO_TCP) == -1)
		return false;

	if (socket.Bind(ip, port) == false)
		return false;

	if (socket.Nonblock() == false)
		return false;

	if (socket.ReuseAddrs() == false)
		return false;

	if (socket.Listen() == false)
		return false;

#ifdef _WINDOWS
	if (ioMux->regist(socket.GetFD()) == false)
		return false;

	if (BeginAccept() == false)
		return false;
#else
	uint32_t events = EPOLLIN | EPOLLHUP | EPOLLERR;
	if (ioMux->regist(socket.GetFD(), events, this) == false)
		return false;
#endif

	return true;
}

#ifdef _WINDOWS
bool Acceptor::BeginAccept()
{
	// OVERLAPPED 구조체를 초기화 시켜주지 않으면, IOCP가 제대로 동작하지 않는다
	acceptOverlapped.Reset();

	// 새로받을 클라이언트 소켓 초기화
	clientSocket.Clear();

	// 사용하진 않지만 매개변수로 넘겨줄 버퍼 초기화
	memset(&acceptBuffer, 0, sizeof(acceptBuffer));
	unsigned long received = 0;

	// Accept될 클라이언트 소켓을 만든다
	clientSocket.Create(SOCK_STREAM, IPPROTO_TCP);

	// 버퍼는 지정해 주지만 크기는 0으로 지정하여 데이터는 받지 않는다. address 필드는 16바이트를 더해 주어야 한다(고 msdn에 나와있다)
	if (INVALID_SOCKET == AcceptEx(socket.GetFD(), clientSocket.GetFD(), acceptBuffer, 0,
								sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &received, static_cast<LPOVERLAPPED>(&acceptOverlapped)))
		return false;

	return true;
}
#endif

#ifdef _WINDOWS
void Acceptor::OnAccept()
{
	// accept가 된 클라이언트 소켓이 listen 소켓의 특징을 물려받도록 한다
	clientSocket.InheritProperties(socket.GetFD());

	// 클라이언트 주소 구함
	sockaddr_in addr;
	int len = sizeof(addr);
	getpeername(clientSocket.GetFD(), (sockaddr*)&addr, &len);

	// 새로운 세션을 만들고
	Session* session = ssMgr->Create(dispatcher);
	
	// 방금 accept된 소켓을 새로만든 세션에 연결해 준다
	session->GetSocket().SetFD(clientSocket.GetFD());

	// IOCP에 세션 소켓을 등록하고
	ioMux->regist(clientSocket.GetFD());

	// 세션의 recv작업 시작하고 사용자 추가작업을 위해 accept되었다고 dispatcher에 알려준다
	session->WaitForRecv();
	session->OnConnect();

	// accept 작업도 다시 시작한다
	BeginAccept();

	LOG_CORE(L"Accepted! ip = %s  port = %d", MbcsToWcs(inet_ntoa(addr.sin_addr)), ntohs((short)addr.sin_port));
}
#else
void Acceptor::OnAccept()
{
	sockaddr_in addr;
	int fd = socket.accept(&addr);
	if (fd == -1)
	{
		LOG_CORE(L"fail accept()");
		return;
	}

	Session* s = _ssMgr->create(_disp);

	s->socket().setSocket(fd);

	LOG_CORE(L"ip = %s  port = %d\n", inet_ntoa(addr.sin_addr),  ntohs((short)addr.sin_port));
}
#endif

void Acceptor::OnError(int err)
{

}

