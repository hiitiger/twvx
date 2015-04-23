#include "stable.h"
#include "twthread.h"

TwThread::TwThread()
    : TwThreadBase()
    , m_loop(nullptr)
{

}

TwThread::~TwThread()
{
    stop();
}

void TwThread::stopQuick()
{
    if (m_threadHandle)
    {
        m_loop->quitQuick();

        DWORD result = ::WaitForSingleObject(m_threadHandle, INFINITE);
        TW_UNUSED(result);
        DAssert(WAIT_OBJECT_0 == result);

        CloseHandle(m_threadHandle);
        m_threadHandle = nullptr;
        m_threadId = k_InvalidId;
    }
}

bool TwThread::postTask( TwTask* task)
{
    if (m_loop)
    {
        m_loop->postTask(task);
        return true;
    }

    return false;
}

bool TwThread::postFunction( const std::function<void()>& func)
{
    if (m_loop)
    {
        m_loop->postFunction(func);
        return true;
    }

    return false;
}

void TwThread::threadMain( )
{
	TwMessageLoop msgLoop;
    m_loop = &msgLoop;
    init();

    m_startEvent->set();

    threadLoop();

    uninit();
    m_loop = nullptr;
}
