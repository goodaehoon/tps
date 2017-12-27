#include "Precompiled.h"
#include "MainThread.h"
#include "SamdaNet.h"
#include "EventObject.h"
#include "Session.h"
#include "SessionManager.h"
#include "IOMultiplexer.h"
#include "MessageQueue.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "Stream.h"
#include "ByteBuffer.h"
#include "Dispatcher.h"

MainThread::MainThread(SamdaNet& samdanet)
	: samdanet(samdanet)
	, interDispatcher(nullptr)
	, timerDispatcher(nullptr)
{
}


void MainThread::SetInterDispatcher(IInterDispatcher* disp)
{
	interDispatcher = disp;
}


void MainThread::SetTimerDispatcher(ITimerDispatcher* disp)
{
	timerDispatcher = disp;
}

void* MainThread::Process()
{
	while (samdanet.IsRunning())
	{
		MessageQueue* msgQueue = samdanet.GetMessageQueue();
		TimerQueue* timerQueue = samdanet.GetTimerQueue();

		if (msgQueue->Empty(QueueType::PACKET_Q) 
			&& msgQueue->Empty(QueueType::INTER_MSG_Q)
			&& timerQueue->Empty())
		{
			Sleep(1);
			continue;
		}
		
		// Ÿ�̸� �޽��� ó��
		{
			Timer timer;
			if (timerQueue->Pop(timer))
			{
				// ������ �ð��� ���� Ÿ�̸Ӱ� �ִ°�?

				// Ÿ�̸Ӹ� ó���Ѵ�
				if (timerDispatcher)
					timerDispatcher->Dispatch(timer);

				// �ݺ��� �����Ǿ� ���� ��� Ÿ�̸Ӹ� �ٽ� �־��ش�
				if (timer.repeat)
					timerQueue->Push(timer);
			}
		}

		// ���͸޽��� ó��
		{
			Stream* stream = msgQueue->Pop(QueueType::INTER_MSG_Q);
			if (stream)
			{
				// Target ������ �ִ� ��� ã�Ƽ� �Ѱ��ش�
				Session* session = samdanet.GetSessionManager()->GetSession(stream->GetCallerID());

				// ���͵���ó�� �����Ǿ� ���� ���� ��찡 �־ null�˻縦 �Ѵ�.
				// �Ѱ��ִ� session�� null�� ��쵵 �ִ�.(Ÿ���� ���ų� ���� ���)
				if (interDispatcher)
					interDispatcher->Dispatch(session, (InterMsg*)stream);
			}
		}

		// Ŭ���̾�Ʈ ��Ŷó��
		{
			Stream* stream = msgQueue->Pop(QueueType::PACKET_Q);
			if (stream)
			{
				Session* session = samdanet.GetSessionManager()->GetSession(stream->GetCallerID());

				session->Dispatch((Packet*)stream);
			}
		}
	}

	return nullptr;
}
