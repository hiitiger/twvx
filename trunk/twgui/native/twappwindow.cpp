#include "stable.h"

#include "twguiinternaldefine.h"
#include "core/twapp.h"

#include "twappwindow.h"

TwAppDriver::TwAppDriver()
: m_init(false)
{
    
}


TwAppDriver::~TwAppDriver()
{

}

bool TwAppDriver::init()
{
    if (m_init)
    {
        return true;
    }

    if(!SimpleWindowBase::Create(L"TwAppDriverWindow",WS_POPUP))
    {
        return false;
    }

    return _init();
}

bool TwAppDriver::registerAppEventId( int id )
{
    if (m_appEventIds.find(id) != m_appEventIds.end())
    {
        m_appEventIds.insert(id);
        return true;
    }
    return false;
}

bool TwAppDriver::_init()
{
    m_init = true;
    return m_init;
}

LRESULT TwAppDriver::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch(uMsg)
    {
    case WM_TW_SENDAPPEVNT:
        if (m_appEventIds.find(wParam) != m_appEventIds.end())
        {
            twApp()->sigTwAppEvent.emit(int(wParam));
        }
        return TRUE;
    }
    return FALSE;
}

int TwAppDriver::sendAppEvent( int id )
{
    if (m_appEventIds.find(id) == m_appEventIds.end())
    {
        return 0;
    }
    return  (int)SendMessage(WM_TW_SENDAPPEVNT,id,0);
}

void TwAppDriver::postAppEvent( int id )
{
    if (m_appEventIds.find(id) == m_appEventIds.end())
    {
        return ;
    }
    PostMessage(WM_TW_SENDAPPEVNT,id,0);
}

