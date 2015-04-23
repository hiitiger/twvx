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
        pThis->runTask();
    }
    else if (uMsg == k_MsgLoopBaseservice)
    {
        pThis->m_baseservice->onServiceMessage((int)wParam, (void*)lParam);
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

struct AsyncFunctionTask : public TwTask
{
    AsyncFunctionTask(const std::function<void()>& func):m_func(func) {;}

    virtual void run() {m_func();}
    std::function<void()> m_func;
};

struct QuitTask : public TwTask
{
    QuitTask(TwMessageLoop* loop, int retCode) :m_loop(loop), m_retCode(retCode) {;}

    virtual void run() 
    {
        *(m_loop->m_running) = false; 
        m_loop->m_retCode = this->m_retCode;
    }

    TwMessageLoop* m_loop;
    int m_retCode;
};

TwMessageLoop::TwMessageLoop()
: m_running(nullptr)
, m_hasPendingTask(false)
, m_retCode(0)
, m_msgWindow(nullptr)
, m_baseservice(new TwBaseService(this))
{
    DAssert_M(!currentLoop(),"error: only one message loop can exist in a thread");
    G_MsgLoopTlsStore.setValue(this);
    initNativeMsgWindow();
}

TwMessageLoop::~TwMessageLoop()
{
    m_baseservice.reset(nullptr);

    ::DestroyWindow(m_msgWindow);
    m_msgWindow = nullptr;
    G_MsgLoopTlsStore.setValue(nullptr);
}

int TwMessageLoop::run(IDispatcher* disp)
{
    bool running = true;;
    bool* prevRunState = m_running;
    m_running = &running ;
    
    runLoop(disp);

    m_running = prevRunState;

    return m_retCode;

}

bool TwMessageLoop::runLoop( IDispatcher* disp /*= nullptr*/)
{
    for(;isRunning();)
    {
        bool hasPendingWork = processMessage(disp);
        if (!isRunning())
        {
            break;
        }

        hasPendingWork |= runTask();
        if (!isRunning())
        {
            break;
        }

        if (!hasPendingWork)
        {
            waitWork();
        }
    }

    return *m_running;
}

void TwMessageLoop::quit( int retCode /*= 0*/ )
{
    postTask(new QuitTask(this, retCode));
}

void TwMessageLoop::quitQuick( int retCode /*= 0*/ )
{
    DAssert(isRunning());
    *m_running = false;
    m_retCode = retCode;
    awake();
}

void TwMessageLoop::postTask( TwTask* task)
{
    postTask(std::shared_ptr<TwTask>(task));
}

void TwMessageLoop::postTask(const std::shared_ptr<TwTask>& task)
{
    TwScopeLovkV1 lock(&m_taskLock);
    m_pendingTaskQueue.push(task);
    m_hasPendingTask = true;
    awakeTask();
}

void TwMessageLoop::postFunction( const std::function<void()>& taskFunc)
{
    postTask(new AsyncFunctionTask(taskFunc));
}

void TwMessageLoop::awake()
{
    ::PostMessage(m_msgWindow, k_MsgLoopWakeMsg, 0, 0);

}

void TwMessageLoop::awakeTask()
{
    ::PostMessage(m_msgWindow, k_MsgLoopTaskMsg, 0, 0);
}

bool TwMessageLoop::processMessage( IDispatcher* disp)
{
    /*
    PeekMessage MSDN:
    During this call, the system delivers pending, non queued messages, 
    that is, messages sent to windows owned by the calling thread.
    */
    bool hasSentMessage = false;

    do 
    {
        DWORD queuestatus = ::GetQueueStatus(QS_SENDMESSAGE);
        if (HIWORD(queuestatus) & QS_SENDMESSAGE)
        {
            hasSentMessage = true;
        }

        MSG msg;

        if (!PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            break;
        }

        if (msg.message == WM_QUIT)
        {
            quitQuick(msg.wParam);
            //Quit Should be posted to outer loop.
            ::PostQuitMessage(static_cast<int>(msg.wParam));
            break;
        }

        if (disp && disp->dispatchWinMessage(msg))
        {

        }
        else
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

//         if (m_hasPendingTask || !isRunning())
//         {
//             break;
//         }
    }while (false);

    return hasSentMessage;
}

bool TwMessageLoop::runTask()
{
    if (!m_hasPendingTask)
    {
        return false;
    }

    if(!loadWorkTask())
    {
        return false;
    }

    m_currentTask->run();
    m_currentTask.reset();

    return m_hasPendingTask;
}


void TwMessageLoop::waitWork()
{
    /*
    WaitMessage MSDN:
    Note that WaitMessage does not return if there is unread input in the message queue after the thread has called a function to check the queue. 
    This is because functions such as PeekMessage, GetMessage, GetQueueStatus, WaitMessage, MsgWaitForMultipleObjects, 
    and MsgWaitForMultipleObjectsEx check the queue and then change the state information for the queue so that the input is no longer considered new. 
    A subsequent call to WaitMessage will not return until new input of the specified type arrives. The existing unread input (received prior to the last time the thread checked the queue) is ignored.
    */
    //::WaitMessage();

    /*
    MsgWaitForMultipleObjectsEx MSDN:
    MWMO_INPUTAVAILABLE 
    The function returns if input exists for the queue, even if the input has been seen (but not removed) using a call to another function, such as PeekMessage.
    */
    ::MsgWaitForMultipleObjectsEx(0, nullptr, INFINITE, QS_ALLINPUT, MWMO_INPUTAVAILABLE);
}

void TwMessageLoop::initNativeMsgWindow()
{
    m_msgWindow = ::CreateWindowW(L"STATIC",L"TwVxMsgLoopWindow",WS_POPUP,0,0,0,0,HWND_MESSAGE,nullptr,nullptr,0);
    ::SetWindowLongPtr(m_msgWindow, GWLP_USERDATA, (LONG_PTR)this);
    ::SetWindowLongPtr(m_msgWindow, GWLP_WNDPROC, (LONG_PTR)TwMessageLoop::LoopWndProc);
}

TwMessageLoop* TwMessageLoop::currentLoop()
{
    return G_MsgLoopTlsStore.getValue();
}

TwBaseService* TwMessageLoop::baseService()
{
    return m_baseservice.get();
}

bool TwMessageLoop::loadWorkTask()
{
    TwScopeLovkV1 lock(&m_taskLock);
    if (m_pendingTaskQueue.empty())
    {
        return false;
    }
    m_currentTask = m_pendingTaskQueue.front();
    m_pendingTaskQueue.pop();
    if (m_pendingTaskQueue.empty())
    {
        m_hasPendingTask = false;
    }
    return true;
}

bool TwMessageLoop::isRunning() const
{
    return m_running && *m_running;
}

