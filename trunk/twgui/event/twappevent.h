#pragma once

#include "twevent.h"

class TwAppEvent : public TwEvent
{
public:
    TwAppEvent(int serviceId);
    virtual ~TwAppEvent();

    int serviceId() const;

private:
    int m_serviceId;
};