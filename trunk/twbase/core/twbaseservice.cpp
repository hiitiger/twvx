#include "stable.h"
#include "native/simplewindowbase.h"
#include "twbaseservice.h"
#include "twmsgloop.h"
#include "twtask.h"
#include "twtimer.h"
#include "twpointer.h"

TwBaseService* twBaseService()
{
    return TwMessageLoop::currentLoop()->baseService();
}

TwBaseService::TwBaseService(TwMessageLoop* loop)
    : m_loop(loop)
    , m_nextTimerId(1)
{
}

TwBaseService::~TwBaseService()
{

}

//////////////////////////////////////////////////////////////////////////
enum BaseServiceMsgId
{
    InvalidMsgId = 0x2014,
};

void TwBaseService::onServiceMessage(int id, void* arg)
{
    TW_UNUSED(id);
    TW_UNUSED(arg);
}
//delete later
struct DeleteLaterTask : public TwTask
{
    DeleteLaterTask(TwObject* obj) : m_obj(obj) {;}
    void run()
    {
        if (m_obj)
        {
            delete m_obj;
        }
    }

    TwWeakPtr<TwObject> m_obj;
};

void TwBaseService::asyncDeleteObject( TwObject* obj )
{
    TwMessageLoop* currentLoop = TwMessageLoop::currentLoop();
    currentLoop->postTask(new DeleteLaterTask(obj));
}

//timer service
void CALLBACK _TimerProc( _In_ HWND , _In_ UINT , _In_ UINT_PTR idEvent, _In_ DWORD  )
{
    twBaseService()->handlerTimerEvent(idEvent);
}

int TwBaseService::nextTimerId()
{
    while(m_timerQueue.find(m_nextTimerId) != m_timerQueue.end())
    {
        m_nextTimerId++;
    }

    return m_nextTimerId++;
}

void TwBaseService::handlerTimerEvent( int timerId )
{
    std::map<int, TwTimer*>::iterator it = m_timerQueue.find(timerId) ;
    it->second->sigTimeOut.emit();
}

bool TwBaseService::killTimer( int timerId )
{
    m_timerQueue.erase(timerId);
    ::KillTimer(nullptr,timerId);

    return true;
}

int TwBaseService::setTimer( int elapseMSecs,TwTimer* timer )
{
    int newTimerId = ::SetTimer(nullptr ,nextTimerId(),elapseMSecs, (TIMERPROC)_TimerProc);
    m_timerQueue.insert(std::pair<int,TwTimer*>(newTimerId,timer));
    return newTimerId;
}

class TimerSink : public TwObject
{
public:
    TwTimer* m_timer;
    std::function<bool()> m_callback;
    bool m_singleShot;
    TimerSink(TwTimer* timer, const std::function<bool()>& callback, bool singleShot)
        : m_timer(timer)
        , m_callback(callback)
        , m_singleShot(singleShot)
    {;}
    ~TimerSink()
    {
        delete m_timer;
        m_timer = nullptr;
    }
    void onTimeOut(TwObject* )
    {
        if (!m_callback() || m_singleShot)
        {
            m_timer->stop();
            TwMessageLoop::currentLoop()->postFunction([this]
            {
                twBaseService()->eraseTimerSink(this);
            });
        }
    }
};

void TwBaseService::startTimer( int milliSeconds, const std::function<bool()>& callback )
{
    TwTimer* timer = new TwTimer();
    TimerSink* timerSink = new TimerSink(timer, callback, false);
    timer->sigTimeOut.connect(timerSink, &TimerSink::onTimeOut);

    twBaseService()->addTimerSink(timerSink);

    timer->start(milliSeconds);
}

void TwBaseService::startSingleShotTimer( int milliSeconds, const std::function<bool()>& callback )
{
    TwTimer* timer = new TwTimer();
    TimerSink* timerSink = new TimerSink(timer, callback, true);
    timer->sigTimeOut.connect(timerSink, &TimerSink::onTimeOut);

    twBaseService()->addTimerSink(timerSink);

    timer->start(milliSeconds);
}

void TwBaseService::addTimerSink( TimerSink* s)
{
    std::shared_ptr<TimerSink> timerSinkPtr(s);
    m_timerSinks.insert(timerSinkPtr);
}

void TwBaseService::eraseTimerSink( TimerSink* s)
{
    auto it = std::find_if(m_timerSinks.begin(), m_timerSinks.end(), 
        [=](const std::shared_ptr<TimerSink>& i)
    {
        return i.get() == s;
    });
    if (it != m_timerSinks.end())
    {
        m_timerSinks.erase(it);
    }
    else
    {
        DAssert(false);
    }
}

