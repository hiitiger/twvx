#include "stable.h"
#include "twtimer.h"
#include "native/internaltimer.h"

TwTimer::TwTimer( TwObject* parent )
: TwObject(parent)
, m_timerId(-1)
, m_elapseMSecs(-1)
{

}

TwTimer::~TwTimer()
{
    stop();
}

void TwTimer::setElapse( int milliseconds )
{
    m_elapseMSecs = milliseconds;
    if (isActive())
    {
        start();
    }
}

void TwTimer::start()
{
    if (m_elapseMSecs != -1)
    {
        start(m_elapseMSecs);
    }
}

void TwTimer::start( int milliseconds )
{
    if (isActive())
    {
        if (m_elapseMSecs == milliseconds)
        {
            return;
        }
        else
        {
            stop();
        }
    }
    
    m_elapseMSecs = milliseconds;
    m_timerId = TwInternalTimer::instance()->setTimer(m_elapseMSecs, this);
}

void TwTimer::stop()
{
    if (m_timerId == -1)
    {
        return;
    }
    TwInternalTimer::instance()->killTimer(m_timerId);
    m_timerId = -1;
    m_elapseMSecs = -1;
}

bool TwTimer::isActive() const
{
    return m_timerId != -1;
}

