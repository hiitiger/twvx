#include "stable.h"
#include "tweventport.h"
#include "twevent.h"

//////////////////////////////////////////////////////////////////////////
TW_CLASS_IMPLEMENT(TwEventPort, TwObject);

TwEventPort::TwEventPort( TwObject* parent /*= nullptr*/ )
: TwObject(parent)
{

}

TwEventPort::~TwEventPort()
{

}

void TwEventPort::installEventHook( TwEventPort* h)
{
    auto it = std::find(m_eventHooks.begin(), m_eventHooks.end(), h);
    if (it != m_eventHooks.end())
    {
        DAssert(false);
        return;
    }
    m_eventHooks.push_back(h);
}

void TwEventPort::removeEventHook( TwEventPort* h)
{
    for(auto it = m_eventHooks.begin(); it != m_eventHooks.end(); ++it)
    {
        if (*it == h)
        {
            m_eventHooks.erase(it);
            break;
        }
    }
}

bool TwEventPort::processEvent( TwEvent* e )
{
    if (filterEventHooks(e))
    {
        return true;
    }
   
    onEvent(e);
    return true;
}

bool TwEventPort::filterEventHooks( TwEvent* e )
{
    if (!m_eventHooks.empty())
    {
        for(EventHooksList::reverse_iterator it = m_eventHooks.rbegin(); it != m_eventHooks.rend();)
        {
            EventHooksList::reverse_iterator itNext = it;
            ++itNext;

            DAssert_M((bool)(*it), "Error: event hook has not be removed before destruct");
            if (*it)
            {
                if((*it)->hookHandleEvent(this,e))
                {
                    return true;
                }
            }

            it = itNext;

        }
    }

    return false;
}

void TwEventPort::onEvent( TwEvent* )
{
    
}

bool TwEventPort::hookHandleEvent( TwEventPort*,TwEvent* )
{
    return false;
}
