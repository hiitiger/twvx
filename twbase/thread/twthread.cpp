#include "stable.h"
#include "twthread.h"
#include "twmsgloop.h"

TwThread::TwThread()
    : TwThreadBase()
    , m_loop(nullptr)
{
    m_createThreadId = std::this_thread::get_id();
}

TwThread::~TwThread()
{
    stop();
}

bool TwThread::start()
{
    DAssert(m_createThreadId == std::this_thread::get_id());

    DAssert(!isRunning());
    DAssert(!isLoopRunning());

    if (__super::start())
    {
        while (!isLoopRunning()) { ; }

        return true;
    }
    return false;
}

void TwThread::stop()
{
    DAssert(m_createThreadId == std::this_thread::get_id());

    if (m_threadHandle)
    {
        m_loop->postFunction([]() {
            TwMessageLoop::current()->quitQuick();
        });

        DWORD result = ::WaitForSingleObject(m_threadHandle, INFINITE);
        (void)(result);
        DAssert(WAIT_OBJECT_0 == result);

        CloseHandle(m_threadHandle);
        m_threadHandle = nullptr;
        m_threadId = k_InvalidId;
    }
}

bool TwThread::postFunctionUncheck(const std::function<void()>& func)
{
    DAssert(m_createThreadId == std::this_thread::get_id());

    if (isLoopRunning())
    {
        m_loop->postFunction(func);
        return true;
    }

    return false;
}

bool TwThread::postFunction(const std::function<void()>& func)
{
    std::lock_guard<std::mutex> lock(m_loopLock);
    if (m_loop && isLoopRunning())
    {
        m_loop->postFunction(func);
        return true;
    }
    return false;
}

bool TwThread::isLoopRunning()
{
    return m_loopRunning;
}

void TwThread::threadMain()
{
    TwMessageLoop msgLoop;
    {
        std::lock_guard<std::mutex> lock(m_loopLock);
        m_loop = &msgLoop;
    }

    init();

    m_loopRunning = true;

    runloop();

    m_loopRunning = false;

    unInit();
    {
        std::lock_guard<std::mutex> lock(m_loopLock);
        m_loop = nullptr;
    }
}


void TwThread::init()
{

}

void TwThread::runloop()
{
    m_loop->run();
}

void TwThread::unInit()
{

}