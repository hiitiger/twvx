#pragma once

#include "twobject.h"
#include "tools/xsigslot.h"

class TW_GUI_API TwTimer : public TwObject
{
public:
    explicit TwTimer(TwObject* parent = NULL);
    virtual ~TwTimer();

    void setElapse(int milliseconds);
    void start();
    void start(int milliseconds);
    void stop();

    bool isActive() const;

public TwSignal:
     sigslot::signal0<> sigTimeOut;

private:
    int m_timerId;
    int m_elapseMSecs;
}; 