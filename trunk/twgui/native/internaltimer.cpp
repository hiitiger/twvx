#include "stable.h"
#include "internaltimer.h"
#include "core/twtimer.h"

#include "twsingleton.h"

TwDefine_SingleTon(TwInternalTimer,_twInternalTimer);

TwInternalTimer::TwInternalTimer()
: m_nextTimerId(1)
{
    if(!SimpleWindowBase::Create(L"TwInternalTimerObjectWindow",WS_POPUP))
    {
        assert(false);
    }
}

int TwInternalTimer::setTimer( int elapseMSecs,TwTimer* timer )
{
    int newTimerId = ::SetTimer(m_hwnd,nextTimerId(),elapseMSecs,NULL);
    m_timerQueue.insert(std::pair<int,TwTimer*>(newTimerId,timer));
    m_nextTimerId = newTimerId;
    return newTimerId;
}

bool TwInternalTimer::killTimer( int timerId )
{
    m_timerQueue.erase(timerId);
    return ::KillTimer(m_hwnd,timerId) == TRUE;
}

BOOL TwInternalTimer::onTimer( WPARAM wParam, LPARAM lParam )
{
    TWUNUSED(lParam);
    int timerId = wParam;
    std::map<int, TwTimer*>::iterator it = m_timerQueue.find(timerId) ;
    if (it == m_timerQueue.end())
    {
        assert(false);
        return FALSE;
    }

    it->second->sigTimeOut.emit();
    return TRUE;
}

int TwInternalTimer::nextTimerId()
{
    while(m_timerQueue.find(m_nextTimerId) != m_timerQueue.end())
    {
        m_nextTimerId++;
    }

    return m_nextTimerId++;
}

TwInternalTimer*  TwInternalTimer::instance()
{
   return _twInternalTimer();
}
