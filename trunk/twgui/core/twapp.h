#pragma once
#include <vector>
#include <string>
#include "twobject.h"
#include "tools/xsigslot.h"
#include "iasynctask.h"

class TwAppDriver;

class TW_GUI_API TwApp : public TwObject, public sigslot::has_slots<>
{
    TW_NONCOPY(TwApp);
public:
    TwApp();
    virtual ~TwApp();

    int run();

    
    void appendAysncTask(IAsyncTask*);
    
    bool registerAppEventId(int id);
    void postAppEvent(int id);

    bool registerInterEventId(int id);

    std::vector<std::wstring> cmdArgs();

public TwSignal:
    sigslot::signal1<int> sigTwAppEvent;

public TwSlot:
    void quit();

private TwSlot:
    //eventloop signal handler
    void onIdle();
    void onAboutQuit();
    void onSxxEvent();

    //
private:
    TwAppDriver* m_appDriver;
};

TW_GUI_API TwApp* twApp();