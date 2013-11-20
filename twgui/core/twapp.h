#pragma once
#include <vector>
#include <string>
#include "twobject.h"
#include "tools/xsigslot.h"



class TW_GUI_API TwApp : public TwObject, public sigslot::has_slots<>
{
    TW_NONCOPY(TwApp);
public:
    TwApp();
    virtual ~TwApp();

    int run();
    void quit();

    void addAsyncTask();

    std::vector<std::wstring> cmdArgs();

private TwSlot:
    //eventloop signal handler
    void onIdle();
    void onAsyncTask();
    void onQuit();
    void onSxxEvent();

    //

private:
};