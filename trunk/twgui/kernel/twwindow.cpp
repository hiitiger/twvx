#include "stable.h"
#include "nativewindow.h"
#include "twwindowmanager.h"
#include "twwindowevendispatch.h"
#include "twrootwidget.h"
#include "twwindow.h"
#include "twpainter.h"
#include "tweventscrack.h"

TW_CLASS_IMPLEMENT(TwWindow, TwControl);

const TwSize<int> k_DefMaxWindowSize = TwSize<int>(1980 * 5, 1080 * 5);

TwWindow::TwWindow( TwObject* parent /*= nullptr*/ )
    : TwControl(parent)
    , sigCreated(this)
    , sigDestroyed(this)
    , sigAboutToClose(this)
    , sigVisibleChanged(this)
    , m_nativeWindow(new TwNativeWindow())
    , m_eventDispatcher(new TwWindowEventDispatcher())
    , m_visible(false)
    , m_maxSize(k_DefMaxWindowSize)
    , m_rootWidget(nullptr)
    , m_title(L"TwWindow")
    , m_windowAttributes(0)
{
    m_windowRect.moveTo(300,200).resize(TwSize<int>(600,400));

    m_rootWidget.reset(new TwRootWidget(this));
    m_rootWidget->setContentMargin(TwMargin<int>());
    
    m_nativeWindow->setTwWindow(this);
    m_eventDispatcher->setTwWindow(this);

    m_nativeWindow->create(nullptr, m_windowRect, m_title);
}

TwWindow::~TwWindow()
{
    //order should not be changed
    m_nativeWindow.reset(nullptr);
    m_rootWidget.reset(nullptr);
    m_eventDispatcher.reset(nullptr);
}

TwRootWidget* TwWindow::rootWidget() const
{
    return m_rootWidget.get();
}

TwWidget* TwWindow::contentWidget() const
{
    return m_rootWidget->contentWidget();
}

TwWidget* TwWindow::findChildWidget(const std::wstring& name)
{
    if (name == m_rootWidget->name())
    {
        return m_rootWidget.get();
    }
    return m_rootWidget->findChildWidget(name);
}

void TwWindow::show()
{
    if (!m_nativeWindow->isWindow())
    {
        m_nativeWindow->create(nullptr, m_windowRect, m_title);
    }

    if (!m_visible)
    {
        m_nativeWindow->show(SW_SHOWNORMAL);
        m_visible = true;
        sigVisibleChanged.emit(m_visible);
    }
}

void TwWindow::showNoActive()
{
    if (!m_nativeWindow->isWindow())
    {
        m_nativeWindow->create(nullptr, m_windowRect, m_title);
    }
    if (!m_visible)
    {
        m_nativeWindow->show(SW_SHOWNOACTIVATE);
        m_visible = true;
        sigVisibleChanged.emit(m_visible);
    }
}

void TwWindow::hide()
{
    if (m_visible)
    {
        m_nativeWindow->hide();
        m_visible = false;
        sigVisibleChanged.emit(m_visible);
    }
}

void TwWindow::setVisible( bool visible )
{
    visible ? show() : hide();
}

bool TwWindow::isVisible() const
{
    return m_visible;
}

bool TwWindow::close()
{
    bool accept = m_nativeWindow->close();
    return accept;
}

void TwWindow::updateWindowRect()
{
    TwSize<int> preferredSize = this->preferredSize();
    TwSize<int> currentSize = size();
    if (currentSize < preferredSize)
    {
        setSize(preferredSize);
    }
}

TwSize<int> TwWindow::preferredSize() const
{
    TwSize<int> preferredSize;
    if (m_rootWidget)
    {
        preferredSize = m_rootWidget->calcLayoutSize();
    }

    TwSize<int> borderSize = m_nativeWindow->getSystemBorderSize();
    preferredSize.setWidth(preferredSize.width() + borderSize.width());
    preferredSize.setHeight(preferredSize.height() + borderSize.height());

    return preferredSize;
}

void TwWindow::setWindowAttribute( Tw::WindowAttribute attr, bool set )
{
    if (set)
    {
        m_windowAttributes |= static_cast<int64>(attr);
    }
    else
    {
        m_windowAttributes &= static_cast<int64>(~attr);
    }
}

bool TwWindow::testWindowAttribute( Tw::WindowAttribute attr ) const
{
    int64 test = m_windowAttributes & static_cast<int64>(attr);
    return test != 0;
}

void TwWindow::activateWindow()
{
    m_nativeWindow->activateWindow();
}

void TwWindow::update()
{
    m_nativeWindow->update();
}

void TwWindow::setPos( const TwPoint<int>& pt )
{
    setWindowRect(windowRect().moveTo(pt));
}

void TwWindow::setPos( int x, int y )
{
    setPos(TwPoint<int>(x ,y));
}

TwPoint<int> TwWindow::pos() const
{
    return windowRect().topLeft();
}

void TwWindow::setSize( const TwSize<int>& sz )
{
    setWindowRect(TwRect<int>(pos(), sz));
}

void TwWindow::setSize( int width, int height )
{
    setSize(TwSize<int>(width, height));
}

TwSize<int> TwWindow::size() const
{
    return windowRect().size();
}

void TwWindow::setMaxSize(int w, int h)
{
    setMaxSize(TwSize<int>(w, h));
}

void TwWindow::setMaxSize(TwSize<int> maxSize)
{
    m_maxSize = maxSize;
}

int TwWindow::width() const
{
    return windowRect().width();
}

void TwWindow::setWidth( int w )
{
    setWindowRect(windowRect().setWidth(w));
}

int TwWindow::height() const
{
    return windowRect().height();
}

void TwWindow::setHeight( int h )
{
    setWindowRect(windowRect().setHeight(h));
}

void TwWindow::setWindowRect( const TwRect<int>& rc )
{
    if (windowRect() != rc)
    {
        m_windowRect = rc;
        m_nativeWindow->setWindowRect(rc);
    }
}

void TwWindow::setWindowRect( int left, int top, int width, int height )
{
    setWindowRect(TwRect<int>(left, top, left+width, top+height));
}

TwRect<int> TwWindow::windowRect() const
{
    return m_windowRect;
}

TwRect<int> TwWindow::localRect() const
{
    return windowRect().movedTo(0,0);
}

TwRect<int> TwWindow::clientRect() const
{
    return m_nativeWindow->getClientRect();;
}

TwRect<int> TwWindow::clientRectInScreen() const
{
    return m_nativeWindow->getClientRectInScreen();
}

TwPoint<int> TwWindow::mapFromWindowToClient( const TwPoint<int>& pt ) const
{
    return m_nativeWindow->mapFromWindowToClient(pt);
}

TwPoint<int> TwWindow::mapFromClientToWindow( const TwPoint<int>& pt ) const
{
    return m_nativeWindow->mapFromClientToWindow(pt);
}

TwPoint<int> TwWindow::mapFromScreenToWindow( const TwPoint<int>& pt ) const
{
    return pt - windowRect().topLeft();
}

TwPoint<int> TwWindow::mapFromWindowToScreen( const TwPoint<int>& pt ) const
{
    return pt + windowRect().topLeft();
}

TwPoint<int> TwWindow::mapFromScreenToClient( const TwPoint<int>& pt ) const
{
    TwPoint<int> ptInWindow = mapFromScreenToWindow(pt);
    return mapFromWindowToClient(ptInWindow);
}

TwPoint<int> TwWindow::mapFromClientToScreen( const TwPoint<int>& pt ) const
{
    TwPoint<int> ptInWindow = mapFromClientToWindow(pt);
    return mapFromWindowToScreen(ptInWindow);
}

void TwWindow::setTitle( const std::wstring& title )
{
    m_title = title;
    m_nativeWindow->setWindowTitle(title);
}

std::wstring TwWindow::title() const
{
    return m_title;
}

int TwWindow::windowId() const
{
    if (!m_nativeWindow->isWindow())
    {
        m_nativeWindow->create(nullptr, m_windowRect, m_title);
    }
    return (int)(m_nativeWindow->Window());
}

ITwRenderSurface* TwWindow::renderSurface() const
{
    return m_renderSurface.get();
}

TwNativeWindow* TwWindow::nativeWindow() const
{
    return m_nativeWindow.get();
}

TwWindowEventDispatcher* TwWindow::eventDispatcher() const
{
    return m_eventDispatcher.get();
}

void TwWindow::initWindowStyle()
{
    //m_nativeWindow->clipSystemBorder(true);
}

bool TwWindow::onWinMessage( UINT msg, WPARAM w, LPARAM l, LRESULT& r )
{
    TW_UNUSED(msg);
    TW_UNUSED(w);
    TW_UNUSED(l);
    TW_UNUSED(r);

    switch (msg)
    {
    case WM_GETMINMAXINFO:
        {
            TwSize<int> preferredSize = this->preferredSize();
            if (preferredSize.width() > m_maxSize.width())
            {
                preferredSize.setWidth(m_maxSize.width());
            }
            if (preferredSize.height() > m_maxSize.height())
            {
                preferredSize.setHeight(m_maxSize.height());
            }

            MINMAXINFO* minmaxInfo = (MINMAXINFO*)l;
            minmaxInfo->ptMinTrackSize.x = preferredSize.width();
            minmaxInfo->ptMinTrackSize.y = preferredSize.height();

            minmaxInfo->ptMaxTrackSize.x = m_maxSize.width();
            minmaxInfo->ptMaxTrackSize.y = m_maxSize.height();

            r = 0;
            return true;
        }
    case WM_SETCURSOR:
        {
            if (LOWORD(l) == HTCLIENT)
            {
                POINT pt;
                GetCursorPos(&pt);
                TwPoint<int> mousePt(pt.x, pt.y);
                TwWidget* w = m_rootWidget->eventHandlerFromPoint(mapFromScreenToClient(mousePt));
                if (w && w->isEnabled())
                {
                    HCURSOR cursorHandle = WindowsCursorS::windowsCursors()->getWindowsCursor(w->cursor().cursorType())->cursorHandle();
                    ::SetCursor(cursorHandle);
                    r = 1;
                    return true;
                }
                
            }
        }
        break;
    default:
        break;
    }
    return false;
}

void TwWindow::onEvent( TwEvent* ev)
{
    TW_EVENTCRACK_BEGIN(ev)

        TW_EVENTSINK_R(ev, TwCreateEvent, Tw::Ev_Create, this, onCreateEvent)
        TW_EVENTSINK_R(ev, TwDestroyEvent, Tw::Ev_Destroy, this, onDestroyEvent)

        TW_EVENTSINK_R(ev, TwSysCmdEvent, Tw::Ev_Close, this, onSysCmdEvent)
        TW_EVENTSINK_R(ev, TwSysCmdEvent, Tw::Ev_Minimize, this, onSysCmdEvent)
        TW_EVENTSINK_R(ev, TwSysCmdEvent, Tw::Ev_Maximize, this, onSysCmdEvent)

        TW_EVENTSINK_R(ev, TwShowEvent, Tw::Ev_Show, this, onShowEvent)
        TW_EVENTSINK_R(ev, TwHideEvent, Tw::Ev_Hide, this, onHideEvent)

        TW_EVENTSINK_R(ev, TwMovingEvent, Tw::Ev_Moving, this, onMovingEvent)
        TW_EVENTSINK_R(ev, TwMoveEvent, Tw::Ev_Move, this, onMoveEvent)

        TW_EVENTSINK_R(ev, TwResizeEvent, Tw::Ev_Resize, this, onResizeEvent)

        TW_EVENTSINK_R(ev, TwPaintEvent, Tw::Ev_Paint, this, onPaintEvent)

        TW_EVENTSINK_R(ev, TwHitTestEvent, Tw::Ev_HitTest, this, onHitTestEvent)

        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MousePress, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseRelease, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseDbClick, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseMove, this, onMouseEvent)

        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverMove, this, onHoverEvent)
        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverLeave, this, onHoverEvent)
        TW_EVENTSINK_R(ev, TwWheelEvent, Tw::Ev_Wheel, this, onWheelEvent)

        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyPress, this, onKeyEvent)
        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyRelease, this, onKeyEvent)
        TW_EVENTSINK_R(ev, TwCharEvent, Tw::Ev_Char, this, onCharEvent)

    TW_EVENTCRACK_END()
}

void TwWindow::onCreateEvent(TwCreateEvent* ev)
{
    TW_UNUSED(ev);
    twApp()->windowManager()->addWindow(this);

    m_renderSurface.reset(nullptr);
    TwD2DRenderSurface* renderSurface = new TwD2DRenderSurface(this);
    m_renderSurface.reset(renderSurface);

    initWindowStyle();

    sigCreated.emit();
}

void TwWindow::onDestroyEvent(TwDestroyEvent* ev)
{
    TW_UNUSED(ev);
    m_visible = false;
    m_renderSurface.reset(nullptr);

    //delete task use post first!
    if (testWindowAttribute(Tw::WA_DeleteOnWindowDestroyed))
    {
        deleteLater();
    }

    twApp()->windowManager()->removeWindow(this);

    sigDestroyed.emit();
}

void TwWindow::onSysCmdEvent( TwSysCmdEvent* ev)
{
    if (ev->id() == Tw::Ev_Close)
    {
        onCloseEvent(static_cast<TwCloseEvent*>(ev));
    }
    else
    {
         ev->accept();
    }
}

void TwWindow::onCloseEvent( TwCloseEvent* ev)
{
    sigAboutToClose.emit();

    ev->accept();
}

void TwWindow::onShowEvent(TwShowEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onHideEvent(TwHideEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onMovingEvent( TwMovingEvent* ev)
{
    TwRect<int> changingRect = ev->preferredRect();
    //TODO: widget process

    TwSize<int> preferredSize = this->preferredSize();
    if (!preferredSize.isZero())
    {
        if (changingRect.width() < preferredSize.width())
        {
            changingRect.setWidth(preferredSize.width());
        }
        if (changingRect.height() < preferredSize.height())
        {
            changingRect.setHeight(preferredSize.height());
        }
    }

    if (changingRect.width() > m_maxSize.width())
    {
        changingRect.setWidth(m_maxSize.width());
    }
    if (changingRect.height() > m_maxSize.height())
    {
        changingRect.setHeight(m_maxSize.height());
    }

    if (changingRect != ev->preferredRect())
    {
        ev->setPreferredRect(changingRect);
        ev->accept();
    }
}

void TwWindow::onMoveEvent( TwMoveEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onResizeEvent( TwResizeEvent* ev)
{
    TwSize<int> newSize = clientRect().size();
    TwSize<int> oldSize = m_rootWidget->size();

    m_renderSurface->resize(newSize);

    m_rootWidget->setBoundRect(clientRect());

    ev->accept();
}

void TwWindow::onPaintEvent(TwPaintEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onHitTestEvent(TwHitTestEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onMouseEvent(TwMouseEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onHoverEvent(TwHoverEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onWheelEvent(TwWheelEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onKeyEvent(TwKeyEvent* ev)
{
    TW_UNUSED(ev);
}

void TwWindow::onCharEvent(TwCharEvent* ev)
{
    TW_UNUSED(ev);
}

