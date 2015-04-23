#include "stable.h"
#include "twevent.h"

static std::set<int> g_RigisteredEvent;

TwEvent::TwEvent(Tw::EventType id)
    : m_evId(id)
    , m_accepted(false)
{

}

TwEvent::~TwEvent()
{

}

int TwEvent::id() const
{
    return m_evId;
}

void TwEvent::accept()
{
    m_accepted = true;
}

void TwEvent::reject()
{
    m_accepted = false;
}

bool TwEvent::accepted() const
{
    return m_accepted;
}

int TwEvent::registerEvent()
{
    int id = Tw::Ev_User;
    while ( g_RigisteredEvent.find(id) != g_RigisteredEvent.end() && id < Tw::Ev_UserMax)
    {
        id += 1;
    }

    if (id < Tw::Ev_UserMax) 
    {
        g_RigisteredEvent.insert(id);
        return id;
    }
    return -1;
}
