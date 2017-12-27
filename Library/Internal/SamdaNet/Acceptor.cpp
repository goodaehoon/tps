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
	// OVERLAPPED ����ü�� �ʱ�ȭ �������� ������, IOCP�� ����� �������� �ʴ´�
	acceptOverlapped.Reset();

	// ���ι��� Ŭ���̾�Ʈ ���� �ʱ�ȭ
	clientSocket.Clear();

	// ������� ������ �Ű������� �Ѱ��� ���� �ʱ�ȭ
	memset(&acceptBuffer, 0, sizeof(acceptBuffer));
	unsigned long received = 0;

	// Accept�� Ŭ���̾�Ʈ ������ �����
	clientSocket.Create(SOCK_STREAM, IPPROTO_TCP);

	// ���۴� ������ ������ ũ��� 0���� �����Ͽ� �����ʹ� ���� �ʴ´�. address �ʵ�� 16����Ʈ�� ���� �־�� �Ѵ�(�� msdn�� �����ִ�)
	if (INVALID_SOCKET == AcceptEx(socket.GetFD(), clientSocket.GetFD(), acceptBuffer, 0,
								sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &received, static_cast<LPOVERLAPPED>(&acceptOverlapped)))
		return false;

	return true;
}
#endif

#ifdef _WINDOWS
void Acceptor::OnAccept()
{
	// accept�� �� Ŭ���̾�Ʈ ������ listen ������ Ư¡�� �����޵��� �Ѵ�
	clientSocket.InheritProperties(socket.GetFD());

	// Ŭ���̾�Ʈ �ּ� ����
	sockaddr_in addr;
	int len = sizeof(addr);
	getpeername(clientSocket.GetFD(), (sockaddr*)&addr, &len);

	// ���ο� ������ �����
	Session* session = ssMgr->Create(dispatcher);
	
	// ��� accept�� ������ ���θ��� ���ǿ� ������ �ش�
	session->GetSocket().SetFD(clientSocket.GetFD());

	// IOCP�� ���� ������ ����ϰ�
	ioMux->regist(clientSocket.GetFD());

	// ������ recv�۾� �����ϰ� ����� �߰��۾��� ���� accept�Ǿ��ٰ� dispatcher�� �˷��ش�
	session->WaitForRecv();
	session->OnConnect();

	// accept �۾��� �ٽ� �����Ѵ�
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

