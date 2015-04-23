#pragma once
#include "twcontrol.h"
/*
    widget layer:
    --common widgets
    --contentWidget, titleBar
    --rootWidget
    --window
    --screen
*/

class TW_GUI_API TwWindow : public TwControl
{
    TW_CLASS_DECLARE(TwWindow);
    friend class TwNativeWindow;
    friend class TwWindowEventDispatcher;
public:

    explicit TwWindow(TwObject* parent = nullptr);
    virtual ~TwWindow();

    TwRootWidget* rootWidget() const;
    TwWidget* contentWidget() const;
    TwWidget* findChildWidget(const std::wstring& name);

    void show();
    void showNoActive();
    void hide();
    void setVisible(bool visible);
    bool isVisible() const;
    bool close();
    void updateWindowRect();
    TwSize<int> preferredSize() const;

    void setWindowAttribute(Tw::WindowAttribute attr, bool set = true);
    bool testWindowAttribute(Tw::WindowAttribute attr) const;

    void activateWindow();
    void update();

    void setPos(const TwPoint<int>& pt);
    void setPos(int x, int y);
    TwPoint<int> pos() const;
    void setSize(const TwSize<int>& sz);
    void setSize(int w, int h);
    TwSize<int> size() const;

    void setMaxSize(int w, int h);
    void setMaxSize(TwSize<int> maxSize);

    int width() const;
    void setWidth(int w);
    int height() const;
    void setHeight(int h);

    //in screen
    void setWindowRect(const TwRect<int>& rc);
    void setWindowRect(int left, int top, int width, int height);
    TwRect<int> windowRect() const;
    //in local
    TwRect<int> localRect() const;

    TwRect<int> clientRect() const;
    TwRect<int> clientRectInScreen() const;

    TwPoint<int> mapFromWindowToClient(const TwPoint<int>& pt) const;
    TwPoint<int> mapFromClientToWindow(const TwPoint<int>& pt) const;

    TwPoint<int> mapFromScreenToWindow(const TwPoint<int>& pt) const;
    TwPoint<int> mapFromWindowToScreen(const TwPoint<int>& pt) const;

    TwPoint<int> mapFromScreenToClient(const TwPoint<int>& pt) const;
    TwPoint<int> mapFromClientToScreen(const TwPoint<int>& pt) const;

    void setTitle(const std::wstring& title);
    std::wstring title() const;
    
    int windowId() const;
    ITwRenderSurface* renderSurface() const;
    TwNativeWindow* nativeWindow() const;
    TwWindowEventDispatcher* eventDispatcher() const;

public :
    TwSignal<void()>        sigCreated;
    TwSignal<void()>        sigDestroyed;
    TwSignal<void()>        sigAboutToClose;
    TwSignal<void(bool)>    sigVisibleChanged;

protected:
    virtual void initWindowStyle();

    virtual bool onWinMessage(UINT msg, WPARAM w, LPARAM l, LRESULT& r);

    virtual void onEvent(TwEvent* );

    virtual void onCreateEvent(TwCreateEvent*);
    virtual void onDestroyEvent(TwDestroyEvent*);

    virtual void onSysCmdEvent(TwSysCmdEvent*);
    virtual void onCloseEvent(TwCloseEvent* );

    virtual void onShowEvent(TwShowEvent*);
    virtual void onHideEvent(TwHideEvent*);

    virtual void onMovingEvent(TwMovingEvent*);
    virtual void onMoveEvent(TwMoveEvent*);

    virtual void onResizeEvent(TwResizeEvent*);

    virtual void onPaintEvent(TwPaintEvent*);

    virtual void onHitTestEvent(TwHitTestEvent*);

    virtual void onMouseEvent(TwMouseEvent*);

    virtual void onHoverEvent(TwHoverEvent*);
    virtual void onWheelEvent(TwWheelEvent*);

    virtual void onKeyEvent(TwKeyEvent*);
    virtual void onCharEvent(TwCharEvent*);

private:
    TwScopedPtr<TwWindowEventDispatcher> m_eventDispatcher;
    TwScopedPtr<TwRootWidget> m_rootWidget;
    TwScopedPtr<TwNativeWindow> m_nativeWindow;
    TwScopedPtr<ITwRenderSurface> m_renderSurface;
    bool m_visible;
    TwRect<int> m_windowRect;
    TwSize<int> m_maxSize;

    std::wstring m_title;

    int64 m_windowAttributes;
};

TW_CLASS_REGISTER(TwWindow);
