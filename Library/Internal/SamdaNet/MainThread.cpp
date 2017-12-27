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
		
		// 타이머 메시지 처리
		{
			Timer timer;
			if (timerQueue->Pop(timer))
			{
				// 지정된 시간이 지난 타이머가 있는가?

				// 타이머를 처리한다
				if (timerDispatcher)
					timerDispatcher->Dispatch(timer);

				// 반복이 설정되어 있을 경우 타이머를 다시 넣어준다
				if (timer.repeat)
					timerQueue->Push(timer);
			}
		}

		// 인터메시지 처리
		{
			Stream* stream = msgQueue->Pop(QueueType::INTER_MSG_Q);
			if (stream)
			{
				// Target 세션이 있는 경우 찾아서 넘겨준다
				Session* session = samdanet.GetSessionManager()->GetSession(stream->GetCallerID());

				// 인터디스패처가 설정되어 있지 않은 경우가 있어서 null검사를 한다.
				// 넘겨주는 session이 null인 경우도 있다.(타겟이 없거나 나간 경우)
				if (interDispatcher)
					interDispatcher->Dispatch(session, (InterMsg*)stream);
			}
		}

		// 클라이언트 패킷처리
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
