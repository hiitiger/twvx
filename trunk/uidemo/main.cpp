#include "stable.h"
#include "twapp.h"
#include "twtimer.h"

INT WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
    TwApp app;
    TwTimer timer;
    timer.setElapse(3000);
    timer.sigTimeOut.connect(&app,&TwApp::quit);
    timer.start();
    app.run();
    return 0;
}