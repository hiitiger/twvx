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
    virtual ~TwTimer();

    // return false for stop timer;
    static void startTimer(int milliSeconds, const std::function<bool()>& callback);
    static void startSingleShotTimer(int milliSeconds, const std::function<bool()>& callback);

    void setElapse(int milliseconds);
    void start();
    void start(int milliseconds);
    void stop();

    bool isActive() const;

public Tw_Signal:
     TwSignal<void()> sigTimeOut;

private:
    int m_timerId;
    int m_elapseMSecs;
}; 

TW_CLASS_REGISTER(TwTimer);