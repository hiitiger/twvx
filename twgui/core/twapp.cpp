#include "stable.h"

#include "twguiinternaldefine.h"
#include "event/msgloop.h"
#include "native/twappwindow.h"
#include "twasynctaskservice.h"
#include "twapp.h"


TwApp* g_TWGLOBAL_TwApp = NULL;

TW_GUI_API TwApp* twApp()
{
    return g_TWGLOBAL_TwApp;
}



TwApp::TwApp()
:TwObject(NULL)
, m_appDriver(new TwAppDriver())
{
    g_TWGLOBAL_TwApp = this;
}

TwApp::~TwApp()
{

}

int TwApp::run()
{
    TwEventLoop eventLoop;
    int ret = eventLoop.run();

    delete m_appDriver;
    m_appDriver = NULL;
    return ret;
}

void TwApp::quit()
{
    PostQuitMessage(0);
}

void TwApp::onIdle()
{
}

void TwApp::onAboutQuit()
{
}

std::vector<std::wstring> TwApp::cmdArgs()
{
    std::vector<std::wstring> argV;
    return argV;
}

bool TwApp::registerInterEventId( int id )
{
    if (id < TwE::EventIdBegin || id > TwE::EventIdEnd)
    {
        return false;
    }
    return m_appDriver->registerAppEventId(id);
}

bool TwApp::registerAppEventId( int id )
{
    if (id < TwE::AppEventIdBegin || id > TwE::AppEventIdEnd)
    {
        return false;
    }
    return m_appDriver->registerAppEventId(id);
}

void TwApp::postAppEvent( int id )
{
    m_appDriver->postAppEvent(id);
}   

void TwApp::appendAysncTask( IAsyncTask* task)
{
    twAsyncTaskService()->appendTask(task);
}



