#include "stable.h"

#include "config/twappconfig.h"
#include "twmsgloop.h"
#include "kernel/twwindowmanager.h"
#include "twpainter.h"
#include "renderer/twrenderresourceprovider.h"

#include "twapp.h"

TwApp* g_TWGLOBAL_TwApp = nullptr;

TwApp* twApp()
{
    return g_TWGLOBAL_TwApp;
}


TwApp::TwApp()
: TwObject(nullptr)
, m_eventLoop(new TwMessageLoop())
, m_windowManager(new TwWindowManager(this))
, m_imagingFactory(nullptr)
{
    g_TWGLOBAL_TwApp = this;

    TwPainter::initRenderSys();

    m_imagingFactory.reset(new TwWICImagingFactory());
    m_rendererResourceProvider.reset(new TwRendererResourceProvider());
}

TwApp::~TwApp()
{
    m_eventLoop.reset(nullptr);
    m_imagingFactory.reset();
    m_rendererResourceProvider.reset();
    TwPainter::unitRenderSys();
    g_TWGLOBAL_TwApp = nullptr;
}

int TwApp::run()
{
    int ret = m_eventLoop->run();
    return ret;
}

void TwApp::quit()
{
    onAboutQuit();
    m_eventLoop->quit(0);
}

void TwApp::onAboutQuit()
{
}

std::vector<std::wstring> TwApp::cmdArgs()
{
    std::vector<std::wstring> argV;
    return argV;
}

TwWindowManager* TwApp::windowManager() const
{
    return m_windowManager.get();
}

TwWICImagingFactory* TwApp::imagingFactory() const
{
    return m_imagingFactory.get();
}

TwRendererResourceProvider* TwApp::rendererResourceProvider() const
{
    return m_rendererResourceProvider.get();
}



