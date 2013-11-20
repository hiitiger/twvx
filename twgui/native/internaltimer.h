#pragma once

#include "SimpleWindowBase.h"
#include <map>
#include "tools/xsigslot.h"

class TwTimer;
class TwInternalTimer : public SimpleWindowBase<TwInternalTimer>
{
public:
    static TwInternalTimer&  instance();
    TwInternalTimer();
    
    int setTimer(int elapseMSecs,TwTimer* timer);
    bool killTimer(int timerId);

public:
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_TIMER)
        {
            return onTimer(wParam,lParam);
        }
        return FALSE;
    }
private:    
    BOOL onTimer(WPARAM wParam, LPARAM lParam);
    int nextTimerId();

private:
    int m_nextTimerId;
    std::map<int,TwTimer*> m_timerQueue;;
};

