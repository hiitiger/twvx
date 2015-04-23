#include "stable.h"
#include "twappevent.h"



TwAppEvent::TwAppEvent(int serviceId)
    : TwEvent(Tw::Ev_AppService)
    , m_serviceId(serviceId)
{

}

TwAppEvent::~TwAppEvent()
{

}

int TwAppEvent::serviceId() const
{
    return m_serviceId;
}
