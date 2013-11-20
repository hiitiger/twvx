#include "stable.h"
#include "twapp.h"
#include "eventloop/msgloop.h"


TwApp::TwApp()
:TwObject(NULL)
{

}

TwApp::~TwApp()
{

}

int TwApp::run()
{
    TwEventLoop eventLoop;
    eventLoop.sigIdle.connect(this,&TwApp::onIdle);
    eventLoop.sigAsyncTask.connect(this,&TwApp::onAsyncTask);
    eventLoop.sigQuit.connect(this,&TwApp::onQuit);
    return eventLoop.run();
}

void TwApp::quit()
{
    PostQuitMessage(0);
}

void TwApp::onIdle()
{

}

void TwApp::onAsyncTask()
{

}

void TwApp::onQuit()
{

}

std::vector<std::wstring> TwApp::cmdArgs()
{
    std::vector<std::wstring> argV;
    return argV;
}

