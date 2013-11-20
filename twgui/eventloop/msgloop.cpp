#include "stable.h"
#include "msgloop.h"


TwEventLoop::TwEventLoop()
{

}

TwEventLoop::~TwEventLoop()
{

}

int TwEventLoop::run()
{
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        sigAsyncTask.emit();
    }
    return msg.wParam;
}


