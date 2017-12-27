#include "Precompiled.h"
#include "Connector.h"
#include "IOMultiplexer.h"
#include "Session.h"
#include "SessionManager.h"
#include "StringUtil.h"

Connector::Connector(IOMultiplexer* ioMux, SessionManager* ssMgr, IDispatcher* disp)
	: ioMux(ioMux)
	, ssMgr(ssMgr)
	, dispatcher(disp)
#ifdef _WINDOWS
	, connectOverlapped(this)
#endif
{
}

bool Connector::Connect(const wchar_t* ip, unsigned short port)
{
	if (socket.Create(SOCK_STREAM, IPPROTO_TCP) == -1)
		return false;

	if (socket.Nonblock() == false)
		return false;

	if (socket.Connect(ip, port) == false)
		return false;

#ifdef _WINDOWS
	if (ioMux->regist(socket.GetFD()) == false)
		return false;

	if (BeginConnect() == false)
		return false;
#else
	uint32_t events = EPOLLIN | EPOLLHUP | EPOLLERR;
	if (ioMux->regist(socket.GetFD(), events, this) == false)
		return false;
#endif

	return true;
}

#ifdef _WINDOWS
bool Connector::BeginConnect()
{
	// OVERLAPPED ����ü�� �ʱ�ȭ �������� ������, IOCP�� ����� �������� �ʴ´�
	connectOverlapped.Reset();

	// ���ι��� Ŭ���̾�Ʈ ���� �ʱ�ȭ
	clientSocket.Clear();

	// ������� ������ �Ű������� �Ѱ��� ���� �ʱ�ȭ
	memset(&acceptBuffer, 0, sizeof(acceptBuffer));
	unsigned long received = 0;

	// Accept�� Ŭ���̾�Ʈ ������ �����
	clientSocket.Create(SOCK_STREAM, IPPROTO_TCP);

	// ���۴� ������ ������ ũ��� 0���� �����Ͽ� �����ʹ� ���� �ʴ´�. address �ʵ�� 16����Ʈ�� ���� �־�� �Ѵ�(�� msdn�� �����ִ�)
//	if (INVALID_SOCKET == ConnectEx(socket.GetFD(), clientSocket.GetFD(), acceptBuffer, 0,
//								sizeof(sockaddr_in)+16, sizeof(sockaddr_in)+16, &received, static_cast<LPOVERLAPPED>(&acceptOverlapped)))
		return false;

	return true;
}
#endif

#ifdef _WINDOWS
void Connector::OnConnect()
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
	BeginConnect();

	LOG_CORE(L"Accepted! ip = %s  port = %d", MbcsToWcs(inet_ntoa(addr.sin_addr)), ntohs((short)addr.sin_port));
}
#else
void Connector::OnConnect()
{
}
#endif

void Connector::OnError(int err)
{

}

