#pragma once

#include "SimpleWindowBase.h"
#include <map>
#include "tools/xsigslot.h"

class TwAppDriver : public SimpleWindowBase<TwAppDriver>
{
public:
    TwAppDriver();
    ~TwAppDriver();
    bool init();
    
    bool registerAppEventId(int id);
    int sendAppEvent(int id);
    void postAppEvent(int id);

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    bool _init();

private:
    bool m_init;
    std::set<int> m_appEventIds;
};

