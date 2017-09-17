#include "stable.h"
#include "twtask.h"
#include "twsingleton.h"
#include "twtlsstore.h"
#include "core/twbaseservice.h"
#include "twsync.h"
#include "base/twbasic.h"
#include "twmsgloop.h"

static TlsStore<TwMessageLoop> G_MsgLoopTlsStore;



LRESULT CALLBACK TwMessageLoop::LoopWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    TwMessageLoop *pThis = (TwMessageLoop*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    DAssert(pThis);pThis;
    if (uMsg == k_MsgLoopTaskMsg)
    {
        pThis->runTaskQueue();
    }
    else if (uMsg == k_MsgLoopWakeMsg)
    {

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



TwMessageLoop::TwMessageLoop()
: m_hasWorkTask(false)
, m_retCode(0)
, m_msgWindow(nullptr)
{
    DAssert_M(!current(),"error: only one message loop can exist in a thread");
    G_MsgLoopTlsStore.setValue(this);
    initNativeMsgWindow();
}

TwMessageLoop::~TwMessageLoop()
{

    ::DestroyWindow(m_msgWindow);
    m_msgWindow = nullptr;
    G_MsgLoopTlsStore.setValue(nullptr);
}

int TwMessageLoop::run()
{
    DAssert(this == current());
    while (isRunning())
    {
        runOnce();

        if (isRunning())
        {
            tryIdleWait();
        }
    }

    return m_retCode;
}


void TwMessageLoop::runOnce()
{
    DAssert(this == current());
    if (isRunning())
        processSystemMessage();

    if (isRunning())
        runTaskQueue();

    if (isRunning())
        runDelayQueue();
}

void TwMessageLoop::tryIdleWait(unsigned int milliSeconds)
{
    DAssert(this == current());
    if (m_hasWorkTask) {
        return;
    }
    auto now = TwTimeTick::now();
    if (!m_nextDelay.isNull())
    {
        unsigned int wait = static_cast<unsigned int>((m_nextDelay - now).milliSecs());
        if (wait > 0)
        {
            wait = min(wait, milliSeconds);
            idleWait(wait);
        }
    }
    else
    {
        idleWait(-1);
    }
}

void TwMessageLoop::idleWait(unsigned int milliSeconds)
{
    DAssert(this == current());

    ::MsgWaitForMultipleObjectsEx(0, nullptr, milliSeconds, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
}

bool TwMessageLoop::isRunning() const
{
    return m_running;
}

void TwMessageLoop::quit( int retCode /*= 0*/ )
{
    if (m_running)
    {
        postFunction([retCode]() {
            TwMessageLoop::current()->m_retCode = retCode;
            TwMessageLoop::current()->m_running = false;

        });
    }
}

void TwMessageLoop::quitQuick( int retCode /*= 0*/ )
{
    DAssert(isRunning());
    m_running = false;
    m_retCode = retCode;
    awake();
}

void TwMessageLoop::postFunction( const std::function<void()>& taskFunc)
{
    add(taskFunc);
}

void TwMessageLoop::postDelayed(const std::function<void()>& func, int milliSeconds)
{
    add({ func, TwTimeTick::now().addMilliSecs(milliSeconds) });
}

void TwMessageLoop::awake()
{
    ::PostMessage(m_msgWindow, k_MsgLoopWakeMsg, 0, 0);
}

void TwMessageLoop::awakeTask()
{
    ::PostMessage(m_msgWindow, k_MsgLoopTaskMsg, 0, 0);
}


void TwMessageLoop::runTaskQueue()
{
    auto now = TwTimeTick::now();
    std::deque<WrapTask> taskQueue = lockSwapTaskQueue();

    for (auto it = taskQueue.begin(); it != taskQueue.end(); ++it)
    {
        if (!isRunning())
        {
            break;
        }
        if (it->run.isNull())
        {
            it->invoke();
        }
        else
        {
            if (now >= it->run)
            {
                it->invoke();
                now = TwTimeTick::now();
            }
            else
            {
                m_delayQueue.push(std::move(*it));
            }
        }
    }

    m_hasWorkTask = m_delayQueue.size() != 0;
}

void TwMessageLoop::runDelayQueue()
{
    auto now = TwTimeTick::now();
    while (true)
    {
        if (m_delayQueue.empty() || m_delayQueue.top().run > now)
        {
            break;
        }
        auto task = std::move(const_cast<WrapTask&>(m_delayQueue.top()));
        m_delayQueue.pop();
        task.invoke();
        now = TwTimeTick::now();
    }

    if (m_delayQueue.empty())
    {
        m_nextDelay.setZero();
    }
    else
    {
        m_nextDelay = m_delayQueue.top().run;
    }
}

void TwMessageLoop::processSystemMessage()
{
    MSG msg = { 0 };
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            m_running = false;
            break;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void TwMessageLoop::add(WrapTask&& task)
{
    if (!m_running) { return; }

    TwScopeLovkV1 lock(&m_taskLock);
    taskSeq_ += 1;
    task.seq_ = taskSeq_;
    m_taskQueue.push_back(std::move(task));
    awakeTask();
}

std::deque<TwMessageLoop::WrapTask> TwMessageLoop::lockSwapTaskQueue()
{
    TwScopeLovkV1 lock(&m_taskLock);
    return std::move(m_taskQueue);
}

void TwMessageLoop::initNativeMsgWindow()
{
    m_msgWindow = ::CreateWindowW(L"STATIC",L"TwVxMsgLoopWindow",WS_POPUP,0,0,0,0,HWND_MESSAGE,nullptr,nullptr,0);
    ::SetWindowLongPtr(m_msgWindow, GWLP_USERDATA, (LONG_PTR)this);
    ::SetWindowLongPtr(m_msgWindow, GWLP_WNDPROC, (LONG_PTR)TwMessageLoop::LoopWndProc);
}

TwMessageLoop* TwMessageLoop::current()
{
    return G_MsgLoopTlsStore.getValue();
}


