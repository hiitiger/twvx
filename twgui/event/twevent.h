#pragma once
#include "twuinamespace.h"

class TW_GUI_API TwEvent
{
public:
    explicit TwEvent(Tw::EventType id);
    virtual ~TwEvent();

    int id() const;

    static int registerEvent();
    void accept();
    void reject();
    bool accepted() const;

private:
    int m_evId;
    bool m_accepted;
};