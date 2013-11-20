#pragma once

#include "core/twobject.h"
#include "tools/xsigslot.h"


class TwEventLoop : public TwObject
{
    TW_NONCOPY(TwEventLoop);
public:
    TwEventLoop();
    virtual ~TwEventLoop();

    int run();

public TwSignal:
    sigslot::signal0<> sigIdle;
    sigslot::signal0<> sigAsyncTask;
    sigslot::signal0<> sigQuit;

private:
};