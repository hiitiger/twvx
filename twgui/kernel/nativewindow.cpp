#include "stable.h"
#include <vector>
#include <algorithm>
#include "twsingleton.h"
#include "twwindow.h"
#include "nativewindow.h"
#include "twpainter.h"
#include "twwindowevendispatch.h"

class  TwClassRegister
{
public:
    TwClassRegister(){;}
    ~ TwClassRegister()
    {
        for( std::vector<std::wstring>::iterator i = m_windowClass.begin(); i != m_windowClass.end(); ++i)
        {
            ::UnregisterClassW(i->c_str(), nullptr);
        }    
    }

    static TwClassRegister* get()
    {
        return TwSingleTonBase<TwClassRegister>::get();
    }

    bool registerClass(const std::wstring& cls)
    {
        if (std::find(m_windowClass.begin(), m_windowClass.end(), cls) != m_windowClass.end())
        {
            return true;
        }

        m_windowClass.push_back(cls);
        return true;
    }

    bool isRegistered(const std::wstring& cls)
    {
       return std::find(m_windowClass.begin(), m_windowClass.end(), cls) != m_windowClass.end();
    }

private:
    std::vector<std::wstring> m_windowClass;
};

//////////////////////////////////////////////////////////////////////////

TwNativeWindow::TwNativeWindow()
    : m_created(false)
    , m_windowStyle(WS_OVERLAPPEDWINDOW )
    , m_windowStyleEx(WS_EX_APPWINDOW )
    , m_windowClass(k_TvxWindow)
    , m_twWindow(nullptr)
    , m_isMouseOver(false)
    , m_isDragging(false)
    , m_pressedButton(Tw::MB_NoButton)
{

}

TwNativeWindow::~TwNativeWindow()
{
    destroy();
}

void TwNativeWindow::setTwWindow( TwWindow* d)
{
    m_twWindow = d;
}

TwWindowEventDispatcher* TwNativeWindow::windowEventDispatcher() const
{
    return m_twWindow->eventDispatcher();
}

void TwNativeWindow::setWindowStyle( DWORD style, DWORD styleEx )
{
    m_windowStyle = style;
    m_windowStyleEx = styleEx;

    if (isWindow())
    {
        ::SetWindowLongPtrW(m_hwnd, GWL_STYLE, m_windowStyle);
        ::SetWindowLongPtrW(m_hwnd, GWL_EXSTYLE, m_windowStyleEx);
    }
}

void TwNativeWindow::setStyle(DWORD style, bool enable)
{
    if (isWindow())
    {
        DWORD curStyle = (DWORD)::GetWindowLongPtrW(m_hwnd,GWL_STYLE);
        if (enable)
        {
            curStyle |= style;
        }
        else
        {
            curStyle &= ~style;
        }
        m_windowStyle = curStyle;
        ::SetWindowLongPtr(m_hwnd,GWL_STYLE,curStyle);
        ::SetWindowPos(m_hwnd,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}

void TwNativeWindow::setStyleEx( DWORD styleEx, bool enable )
{
    if (isWindow())
    {
        DWORD curStyleEx = (DWORD)::GetWindowLongPtrW(m_hwnd,GWL_EXSTYLE);
        if (enable)
        {
            curStyleEx |= styleEx;
        }
        else
        {
            curStyleEx &= ~styleEx;
        }
        m_windowStyle = curStyleEx;
        ::SetWindowLongPtr(m_hwnd,GWL_STYLE, curStyleEx);
        ::SetWindowPos(m_hwnd,0,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}

void TwNativeWindow::clipSystemBorder( bool  clip)
{
    if (clip)
    {
        setStyle(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU, false);
    }
    else
    {
        setStyle(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU, true);
    }
}

void TwNativeWindow::create( HWND parent, const TwRect<int>& rc, const std::wstring& title )
{
    if (m_created)
    {
        return;
    }

    if (!TwClassRegister::get()->isRegistered(m_windowClass))
    {
        RegisterWindowClass();
        TwClassRegister::get()->registerClass(m_windowClass);
    }

    __super::Create(nullptr, m_windowStyle, m_windowStyleEx, rc.left(), rc.top(), rc.width(), rc.height(),parent, 0);
    m_created = true;

    setWindowTitle(title);

    //create event send after createwindow return
    TwCreateEvent ev;
    windowEventDispatcher()->dispatchEvent(&ev);
}

void TwNativeWindow::destroy()
{
    if (m_created)
    {
        __super::DestroyWindow();
    }
}

bool TwNativeWindow::isWindow() const
{
#ifdef _DEBUG
    return (::IsWindow(m_hwnd) == TRUE);
#else
    return m_hwnd != nullptr;
#endif // _DEBUG
}

void TwNativeWindow::setWindowTitle( const std::wstring& title)
{
    if (isWindow())
    {
        ::SetWindowTextW(m_hwnd, title.c_str());
    }
}

void TwNativeWindow::show(DWORD nCmd)
{
    if (isWindow())
    {
        TwShowEvent ev;
        windowEventDispatcher()->dispatchEvent(&ev);
//      if (ev.accepted())
//      {
            m_twWindow->updateWindowRect();
            ShowWindow(m_hwnd, nCmd);
//      }
    }
}

void TwNativeWindow::hide()
{
    if (isWindow())
    {
        TwHideEvent ev;
        windowEventDispatcher()->dispatchEvent(&ev);
//      if (ev.accepted())
//      {
            ::ShowWindow(m_hwnd, SW_HIDE);
//      }
    }
}

bool TwNativeWindow::close()
{
    if (isWindow())
    {
        TwCloseEvent ev;
        windowEventDispatcher()->dispatchEvent(&ev);
        if (ev.accepted())
        {
            destroy();
            return true;
        }
    }
    return false;
}

bool TwNativeWindow::isVisibleInNativeSys() const
{
    if (!isWindow())
    {
        return ::IsWindowVisible(m_hwnd) == TRUE;
    }
    return false;
}

void TwNativeWindow::activateWindow()
{
    if (isWindow())
    {
        ::ShowWindow(m_hwnd, SW_SHOW);
        ::SetActiveWindow(m_hwnd);
    }
}

void TwNativeWindow::update( const TwRect<int> rc /*= TwRect<int>()*/ )
{
    if (isWindow())
    {
        if (rc.isValid())
        {
            ::InvalidateRect(m_hwnd, &(twRectToWinRect(rc)), TRUE);
        }
        else
        {
            ::InvalidateRect(m_hwnd, nullptr, TRUE);
        }
    }
}

void TwNativeWindow::moveWindow( int x, int y )
{
    if (isWindow())
    {
        ::SetWindowPos(m_hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

void TwNativeWindow::setWindowRect( const TwRect<int>& rc )
{
    if (isWindow())
    {
        ::MoveWindow(m_hwnd, rc.left(), rc.top(), rc.width(), rc.height(), FALSE);
    }
}

TwRect<int> TwNativeWindow::getWindowRect() const
{
    RECT rect = {0};
    if (isWindow())
    {
        ::GetWindowRect(m_hwnd, &rect);
    }
    return TwRect<int>(rect.left, rect.top, rect.right, rect.bottom);
}

TwRect<int> TwNativeWindow::getClientRect() const
{
    RECT rect = {0};
    if (isWindow())
    {
        ::GetClientRect(m_hwnd, &rect);
    }
    return TwRect<int>(rect.left, rect.top, rect.right, rect.bottom);
}

TwRect<int> TwNativeWindow::getClientRectInScreen() const
{
    RECT rect = {0};
    AdjustWindowRect(&rect, (DWORD)GetWindowLongPtr(m_hwnd, GWL_STYLE), FALSE);

    TwRect<int> rc(rect.left, rect.top, rect.right, rect.bottom);
    TwRect<int> windowRect = getWindowRect();

    int clientLeft = windowRect.left() - rc.left();
    int clientTop = windowRect.top() - rc.top();
    int clientRight = clientLeft + windowRect.right() -windowRect.left()- (rc.right() - rc.left());
    int clientBottom = clientTop + windowRect.bottom() -windowRect.top()- (rc.bottom() - rc.top());

    TwRect<int> ret(clientLeft, clientTop, clientRight, clientBottom);
    return ret;
}

TwSize<int> TwNativeWindow::getSystemBorderSize() const
{
    RECT rect = { 0 };
    AdjustWindowRect(&rect, (DWORD)GetWindowLongPtr(m_hwnd, GWL_STYLE), FALSE);
    return TwSize<int>(rect.right - rect.left, rect.bottom - rect.top);
}

TwPoint<int> TwNativeWindow::mapFromClientToWindow( const TwPoint<int>& pt ) const
{
    RECT rect = {0};
    AdjustWindowRect(&rect, (DWORD)GetWindowLongPtr(m_hwnd, GWL_STYLE), FALSE);
    TwPoint<int> point;
    point.setX(pt.x() - rect.left);
    point.setY(pt.y() - rect.top);
    return point;
}

TwPoint<int> TwNativeWindow::mapFromWindowToClient( const TwPoint<int>& pt ) const
{
    RECT rect = {0};
    AdjustWindowRect(&rect, (DWORD)GetWindowLongPtr(m_hwnd, GWL_STYLE), FALSE);
    TwPoint<int> point;
    point.setX(pt.x() + rect.left);
    point.setY(pt.y() + rect.top);
    return point;
}

PCWSTR TwNativeWindow::ClassName() const
{
    return m_windowClass.c_str();
}


//////////////////////////////////////////////////////////////////////////
LRESULT TwNativeWindow::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    LRESULT res = 0;
    if (m_twWindow->onWinMessage(uMsg, wParam, lParam, res))
    {
        return res;
    }

    if( xxProcessMessage(uMsg, wParam, lParam, res, 0))
    {
        return res;
    }

    res = DefWindowProc(m_hwnd, uMsg, wParam, lParam);
    return res;
}

bool TwNativeWindow::onCreate( UINT , WPARAM , LPARAM , LRESULT&  )
{
    return false;
}

bool TwNativeWindow::onClose( UINT , WPARAM , LPARAM , LRESULT& res )
{
    TwCloseEvent ev;
    windowEventDispatcher()->dispatchEvent(&ev);
    if (ev.accepted())
    {
        destroy();
    }
    res = 0;
    return true;
}

bool TwNativeWindow::onDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    return false;
}

bool TwNativeWindow::onNcDestroy( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    m_created = false;
    TwDestroyEvent ev;
    windowEventDispatcher()->dispatchEvent(&ev);
    m_hwnd = nullptr;

    return false;
}

bool TwNativeWindow::onShowWindow( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    //Native WM_Show msg does not cause show or hide TwEvent dispatch.
    //DCLog(TwXXXLog)<<"onShowWindow : "<<(uMsg ? "Show" : "hide");
    //ReleaseLog(TwXXXLog)<<"onShowWindow : "<<(uMsg ? "Show" : "hide");
    return false;
}

bool TwNativeWindow::onMoving( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    LPRECT lpRect = (LPRECT)lParam;
    TwRect<int> preferredRect(lpRect->left, lpRect->top,lpRect->right, lpRect->bottom);
    TwMovingEvent ev(preferredRect);
    windowEventDispatcher()->dispatchEvent(&ev);
    if (ev.accepted())
    {
        lpRect->left = ev.preferredRect().left();
        lpRect->top = ev.preferredRect().top();
        lpRect->right = ev.preferredRect().right();
        lpRect->bottom = ev.preferredRect().bottom();
        res  = TRUE;
        return true;
    }
    return false;
}

bool TwNativeWindow::onSize( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwRect<int> newRc = getWindowRect();
    TwResizeEvent ev(newRc.size(), m_twWindow->size());
    m_twWindow->m_windowRect = newRc;

    windowEventDispatcher()->dispatchEvent(&ev);
    
    return false;
}

bool TwNativeWindow::onMove( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwPoint<int> newPos = getWindowRect().topLeft();
    TwPoint<int> oldPos = m_twWindow->m_windowRect.topLeft();
    
    //just move
    //while maximum a window, move msg come before size msg, if set rect here, handle size msg will get wrong old size
    m_twWindow->m_windowRect.moveTo(newPos);

    TwMoveEvent ev(newPos, oldPos);
    windowEventDispatcher()->dispatchEvent(&ev);
    
    return false;
}

bool TwNativeWindow::onSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);

    TwFocusEvent ev(Tw::Ev_FocusIn);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);
    TwFocusEvent ev(Tw::Ev_FocusOut);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onPaint( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    PAINTSTRUCT ps;
    ::BeginPaint(m_hwnd, &ps);

    TwPainter painter(m_twWindow->renderSurface());
    TwPaintEvent ev(TwPaintEvent::ContentPaint, &painter/*, TwRect<int>(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom)*/);
    windowEventDispatcher()->dispatchEvent(&ev);

    ::EndPaint(m_hwnd, &ps);

    return true;
}

bool TwNativeWindow::onNcPaint( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwPainter painter(m_twWindow->renderSurface());
    TwPaintEvent ev(TwPaintEvent::NcPaint, &painter);
    windowEventDispatcher()->dispatchEvent(&ev);
    if (ev.accepted())
    {
        res = 0;
        return true;
    }
    else
    {
        return false;
    }
}

bool TwNativeWindow::onEraseBkgnd( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    TwPainter painter(m_twWindow->renderSurface());
    TwPaintEvent ev(TwPaintEvent::EraseBackground, &painter);
    windowEventDispatcher()->dispatchEvent(&ev);
    if (ev.accepted())
    {
        res = 1;
        return true;
    }
    else
    {
        return false;
    }
}

bool TwNativeWindow::onNcHitTest( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);

    TwPoint<int> point((int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam));
    point.setX(point.x() - m_twWindow->windowRect().left());
    point.setY(point.y() - m_twWindow->windowRect().top());

    TwHitTestEvent ev(point);
    windowEventDispatcher()->dispatchEvent(&ev);

    if (ev.accepted())
    {
        switch (ev.result())
        {
        case TwHitTestEvent::Border:
            res = HTBORDER;
            return true;
        case TwHitTestEvent::Top:
            res = HTTOP;
            return true;
        case TwHitTestEvent::Bottom:
            res = HTTOP;
            return true;
        case TwHitTestEvent::Left:
            res = HTLEFT;
            return true;
        case TwHitTestEvent::Right:
            res = HTRIGHT;
            return true;
        case TwHitTestEvent::BottomLeft:
            res = HTBOTTOMLEFT ;
            return true;
        case TwHitTestEvent::BottomRight:
            res = HTBOTTOMRIGHT;
            return true;
        case TwHitTestEvent::TopLeft:
            res = HTTOPLEFT ;
            return true;
        case TwHitTestEvent::TopRight:
            res = HTTOPRIGHT ;
            return true;
        case TwHitTestEvent::Caption:
            res = HTCAPTION;
            return true;
        case TwHitTestEvent::Client:
            res = HTCLIENT;
            return true;
        case TwHitTestEvent::MinimumButton:
            res = HTMINBUTTON;
            return true;
        case TwHitTestEvent::MaximumButton:
            res = HTMAXBUTTON;
            return true;
        case TwHitTestEvent::CloseButton:
            res = HTCLOSE;
            return true;
        case TwHitTestEvent::SysMenu:
            res = HTSYSMENU;
            return true;
        default:
            break;
        } 
    }
    return false;
}

bool isButtonDown(WPARAM wParam)
{
    return (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) != 0;
}

bool TwNativeWindow::onMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    if (!m_isMouseOver)
    {
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof (tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hwnd;
        tme.dwHoverTime = 0;

        ::TrackMouseEvent (&tme);
        m_isMouseOver = true;
    }

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);
    if (m_isDragging)
    {
        TwMouseEvent ev(Tw::Ev_MouseMove, screenPos, clientPos, mapFromClientToWindow(clientPos), m_pressedButton);
        windowEventDispatcher()->dispatchEvent(&ev);
    }
    else
    {
        TwHoverEvent ev(Tw::Ev_HoverMove, screenPos, clientPos, mapFromClientToWindow(clientPos));
        windowEventDispatcher()->dispatchEvent(&ev);
    }

    return false;
}

bool TwNativeWindow::onMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    if (m_isMouseOver)
    {
        m_isMouseOver = false;
    }

    DWORD msgPos = ::GetMessagePos();
    POINT pt = {(int)(short) LOWORD(msgPos), (int)(short) HIWORD(msgPos)};
    TwPoint<int> screenPos(pt.x, pt.y);
    ::ScreenToClient(m_hwnd, &pt);
    TwPoint<int> clientPos(pt.x, pt.y);
    TwHoverEvent ev(Tw::Ev_HoverLeave, screenPos, clientPos, mapFromClientToWindow(clientPos));

    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    SetCapture(m_hwnd);
    m_isDragging = true;
    m_pressedButton = Tw::MB_LeftButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_LeftButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onLButtonDBClick( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    SetCapture(m_hwnd);
    m_isDragging = true;
    m_pressedButton = Tw::MB_LeftButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent evDbClick(Tw::Ev_MouseDbClick, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_LeftButton);
    windowEventDispatcher()->dispatchEvent(&evDbClick);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_LeftButton);
    windowEventDispatcher()->dispatchEvent(&ev);
    return false;
}

bool TwNativeWindow::onLButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    m_isDragging = false;
    m_pressedButton = Tw::MB_NoButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MouseRelease, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_LeftButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    ::ReleaseCapture();
    return false;
}

bool TwNativeWindow::onRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam , LRESULT&  res)
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    ::SetCapture(m_hwnd);
    m_isDragging = true;
    m_pressedButton = Tw::MB_RightButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_RightButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onRButtonDBClick( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    SetCapture(m_hwnd);
    m_isDragging = true;
    m_pressedButton = Tw::MB_RightButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);


    TwMouseEvent evDbClick(Tw::Ev_MouseDbClick, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_RightButton);
    windowEventDispatcher()->dispatchEvent(&evDbClick);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_RightButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onRButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    m_isDragging = false;
    m_pressedButton = Tw::MB_NoButton;

    POINT pt = {(int)(short) LOWORD(lParam), (int)(short) HIWORD(lParam)};
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MouseRelease, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_RightButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    ::ReleaseCapture();
    return false;
}

bool TwNativeWindow::onMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);

    ::SetCapture(m_hwnd);
    m_isDragging = true;
    m_pressedButton = Tw::MB_MidButton;

    POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_MidButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onMButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);

    POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MousePress, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_MidButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    TwMouseEvent evDbClick(Tw::Ev_MouseDbClick, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_MidButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);

    m_isDragging = false;
    m_pressedButton = Tw::MB_NoButton;

    POINT pt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
    TwPoint<int> clientPos(pt.x, pt.y);
    ::ClientToScreen(m_hwnd, &pt);
    TwPoint<int> screenPos(pt.x, pt.y);

    TwMouseEvent ev(Tw::Ev_MouseRelease, screenPos, clientPos, mapFromClientToWindow(clientPos), Tw::MB_MidButton);
    windowEventDispatcher()->dispatchEvent(&ev);

    ::ReleaseCapture();
    return false;
}

bool TwNativeWindow::onMouseCaputureChanged( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);
    if ((HWND)lParam != m_hwnd)
    {
        m_isDragging = false;
        m_pressedButton = Tw::MB_NoButton;

        DWORD msgPos = ::GetMessagePos();
        POINT pt = {(int)(short) LOWORD(msgPos), (int)(short) HIWORD(msgPos)};
        TwPoint<int> screenPos(pt.x, pt.y);
        ::ScreenToClient(m_hwnd, &pt);
        TwPoint<int> clientPos(pt.x, pt.y);

        TwMouseEvent ev(Tw::Ev_MouseRelease, screenPos, clientPos, mapFromClientToWindow(clientPos));
        windowEventDispatcher()->dispatchEvent(&ev);
    }
    return false;
}

bool TwNativeWindow::onWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res)
{
    TW_UNUSED(uMsg); TW_UNUSED(wParam); TW_UNUSED(lParam); TW_UNUSED(res);

    POINT screenPt = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };
    TwPoint<int> screenPos(screenPt.x, screenPt.y);
    ::ScreenToClient(m_hwnd, &screenPt );
    TwPoint<int> clientPos(screenPt.x, screenPt.y);

    TwWheelEvent ev(((short)HIWORD(wParam)), screenPos, clientPos, clientPos);
    windowEventDispatcher()->dispatchEvent(&ev);
    res = 0;
    return true;
}

bool TwNativeWindow::onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwKeyEvent ev(Tw::Ev_KeyPress);
    ev.m_key      = (int)wParam;
    ev.m_ctrl     = Windows::isKeyDown(VK_CONTROL);
    ev.m_shift    = Windows::isKeyDown(VK_SHIFT);
    ev.m_alt      = Windows::isKeyDown(VK_MENU);
    ev.m_capslock = Windows::isCapsLock();

    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onKeyUp( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwKeyEvent ev(Tw::Ev_KeyRelease);
    ev.m_key      = (int)wParam;
    ev.m_ctrl     = Windows::isKeyDown(VK_CONTROL);
    ev.m_shift    = Windows::isKeyDown(VK_SHIFT);
    ev.m_alt      = Windows::isKeyDown(VK_MENU);
    ev.m_capslock = Windows::isCapsLock();

    windowEventDispatcher()->dispatchEvent(&ev);

    return false;
}

bool TwNativeWindow::onChar( UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT&  res )
{
    TW_UNUSED(uMsg);TW_UNUSED(wParam);TW_UNUSED(lParam);TW_UNUSED(res);

    TwCharEvent ev;
    ev.m_char = (wchar_t)wParam;
    ev.m_ctrl     = Windows::isKeyDown(VK_CONTROL);
    ev.m_shift    = Windows::isKeyDown(VK_SHIFT);
    ev.m_alt      = Windows::isKeyDown(VK_MENU);
    ev.m_capslock = Windows::isCapsLock();

    windowEventDispatcher()->dispatchEvent(&ev);
    return false;
}

