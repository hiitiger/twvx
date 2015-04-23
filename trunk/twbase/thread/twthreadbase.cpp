#include "stable.h"
#include <process.h>
#include "twthreadbase.h"
#include "twsync.h"

unsigned int __stdcall TwThreadBase::_threadProc( void* arg)
{
    __try
    {
        TwThreadBase* pThis = (TwThreadBase*) arg;
        pThis->threadMain();
    }
    __except(UnhandledExceptionFilter(GetExceptionInformation()))
    {
        ;
    }
	return 0;
}


TwThreadBase::TwThreadBase()
    : m_threadId(k_InvalidId)
    , m_threadHandle(nullptr)
    , m_startEvent(nullptr)
{

}

TwThreadBase::~TwThreadBase()
{
    stop();
}

bool TwThreadBase::start()
{
    m_startEvent.reset(new TwWaitableEvent());

    m_threadHandle = (HANDLE)_beginthreadex(nullptr, 0, _threadProc, (void*)this, 0, &m_threadId);
    if (!m_threadHandle)
    {
        return false;
    }
    m_startEvent->wait();
    m_startEvent.reset(nullptr);
    return true;
}

void TwThreadBase::stop()
{
    if (m_threadHandle)
    {
        PostThreadMessageW(m_threadId, WM_QUIT, 0, 0);

        DWORD result = ::WaitForSingleObject(m_threadHandle, INFINITE);
        TW_UNUSED(result);
        DAssert(WAIT_OBJECT_0 == result);

        CloseHandle(m_threadHandle);
        m_threadHandle = nullptr;
        m_threadId = k_InvalidId;
    }
}
