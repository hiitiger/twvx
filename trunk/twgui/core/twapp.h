#pragma once

class TwWindowManager;
class TwWICImagingFactory;
class TwRendererResourceProvider;

class TW_GUI_API TwApp : public TwObject
{
    TW_DISABLE_COPY(TwApp);
public:
    TwApp();
    virtual ~TwApp();

    int run();
    void quit();

    std::vector<std::wstring> cmdArgs();

    TwWindowManager* windowManager() const; 
    TwWICImagingFactory* imagingFactory() const;
    TwRendererResourceProvider* rendererResourceProvider() const;

protected :
    virtual void onAboutQuit();

    //
private:
    TwScopedPtr<TwMessageLoop> m_eventLoop;
    TwScopedPtr<TwWindowManager> m_windowManager;
    TwScopedPtr<TwWICImagingFactory> m_imagingFactory;
    TwScopedPtr<TwRendererResourceProvider> m_rendererResourceProvider;
};

TW_GUI_API TwApp* twApp();