#include "stable.h"
#include "twtimer.h"

    class TimerCallback
    {
        friend class TwTimer;
        TwTimer* timer_;
    public:
        TimerCallback(TwTimer* timer) : timer_(timer)
        {

        }

        void run()
        {
            if (timer_)
            {
                timer_->sigTimeout().emit();

                if (timer_ && timer_->m_repeat)
                {
                    timer_->queueTimerTaskNext();
                }
            }
        }
    };

    TW_CLASS_IMPLEMENT(TwTimer, TwObject);

    TwTimer::TwTimer(TwObject* parent)
        : TwObject(parent)
        , m_sigTimeout(this)
    {

    }

    TwTimer::~TwTimer()
    {
        stop();
    }

    void TwTimer::setInterval(int milliSeconds)
    {
        if (m_intervalMilliSeconds != milliSeconds)
        {
            if (isActive())
            {
                stop();
                start(m_intervalMilliSeconds);
            }
            else
                m_intervalMilliSeconds = milliSeconds;
        }
    }

    void TwTimer::setRepeat(bool r)
    {
        DAssert(!isActive());
        m_repeat = r;
    }


    void TwTimer::startSingleShot()
    {
        setRepeat(false);
        start();
    }

    void TwTimer::startSingleShot(int milliSeconds)
    {
        setRepeat(false);
        start(milliSeconds);
    }

    void TwTimer::start()
    {
        if (m_intervalMilliSeconds != -1)
        {
            start(m_intervalMilliSeconds);
        }
    }

    void TwTimer::start(int milliSeconds)
    {
        if (isActive())
        {
            if (m_intervalMilliSeconds = milliSeconds)
            {
                return;
            }
            else
                stop();
        }

        m_intervalMilliSeconds = milliSeconds;
        queueTimerTask();
    }

    void TwTimer::stop()
    {
        if (isActive())
        {
            m_callback->timer_ = nullptr;
            m_callback = nullptr;
        }
    }

    bool TwTimer::isActive() const
    {
        return (bool)m_callback;
    }

    TwSignal<void()>&  TwTimer::sigTimeout()
    {
        return m_sigTimeout;
    }

    void TwTimer::queueTimerTask()
    {
        DAssert(!isActive());
        m_callback = std::make_shared<TimerCallback>((this));
        TwMessageLoop::current()->postDelayed(std::bind(&TimerCallback::run, m_callback), m_intervalMilliSeconds);
    }

    void TwTimer::queueTimerTaskNext()
    {
        DAssert(isActive());
        TwMessageLoop::current()->postDelayed(std::bind(&TimerCallback::run, m_callback), m_intervalMilliSeconds);
    }

