#pragma once

#include "twobject.h"

//////////////////////////////////////////////////////////////////////////
/*

*/
class TW_BASE_API TwTimer : public TwObject
{
    TW_CLASS_DECLARE(TwTimer);

public:
    explicit TwTimer(TwObject* parent = nullptr);

    ~TwTimer();

    void setInterval(int milliSeconds);

    void setRepeat(bool);

    void startSingleShot();

    void startSingleShot(int milliSeconds);

    void start();

    void start(int milliSeconds);

    void stop();

    bool isActive() const;

    TwSignal<void()>& sigTimeout();

protected:
    void queueTimerTask();

    void queueTimerTaskNext();

private:
    friend class TimerCallback;

    int m_intervalMilliSeconds = -1;
    bool m_repeat = true;
    std::shared_ptr<TimerCallback> m_callback;
    TwSignal<void()> m_sigTimeout;
};


TW_CLASS_REGISTER(TwTimer);