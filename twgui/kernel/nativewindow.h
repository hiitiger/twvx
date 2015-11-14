#pragma once
#include "simplewindowbase.h"
#include "twmessagecrack.h"
#include "windowshelper.h"

static const wchar_t k_TvxWindow[] = L"TvxWindow";

class TwWindow;

class  TwNativeWindow :  public SimpleWindowBase<TwNativeWindow>
{
    TW_DISABLE_COPY(TwNativeWindow)
public:
    TwNativeWindow();
    virtual ~ TwNativeWindow();

    void setTwWindow(TwWindow*);
    TwWindowEventDispatcher* windowEventDispatcher() const;

    void setWindowStyle(DWORD style, DWORD styleEx);
    void setStyle(DWORD style, bool enable );
    void setStyleEx(DWORD styleEx, bool enable );

    void clipSystemBorder(bool );

    void create(HWND parent, const TwRect<int>& rc, const std::wstring& title = L"");
    
    void destroy();

    bool isWindow() const;

    void setWindowTitle(const std::wstring& );

    void show(DWORD nCmd);
    void hide();
    bool close();
    bool isVisibleInNativeSys() const;
    void activateWindow();
    void update(const TwRect<int> rc = TwRect<int>());

    void moveWindow(int x, int y);
    void setWindowRect(const TwRect<int>& rc);
    TwRect<int> getWindowRect() const;
    TwRect<int> getClientRect() const;
    TwRect<int> getClientRectInScreen() const;
    TwSize<int> getSystemBorderSize() const;

    TwPoint<int> mapFromClientToWindow(const TwPoint<int>& pt) const;
    TwPoint<int> mapFromWindowToClient(const TwPoint<int>& pt) const;


    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    TW_MESSAGEMAP_BEGIN(TwNativeWindow)
        //key window msg
        TW_MESSAGE_HANDLER(WM_CREATE,onCreate)
        TW_MESSAGE_HANDLER(WM_CLOSE,onClose)
        TW_MESSAGE_HANDLER(WM_DESTROY,onDestroy)
        TW_MESSAGE_HANDLER(WM_NCDESTROY,onNcDestroy)
        TW_MESSAGE_HANDLER(WM_SHOWWINDOW,onShowWindow)
        TW_MESSAGE_HANDLER(WM_SIZING,onMoving)
        TW_MESSAGE_HANDLER(WM_MOVING,onMoving)
        TW_MESSAGE_HANDLER(WM_SIZE,onSize)
        TW_MESSAGE_HANDLER(WM_MOVE,onMove)

        TW_MESSAGE_HANDLER(WM_SETFOCUS, onSetFocus)
        TW_MESSAGE_HANDLER(WM_KILLFOCUS, onKillFocus)

        //Paint
        TW_MESSAGE_HANDLER(WM_PAINT, onPaint)
        TW_MESSAGE_HANDLER(WM_NCPAINT, onNcPaint)
        TW_MESSAGE_HANDLER(WM_ERASEBKGND, onEraseBkgnd)

        //Mouse
        TW_MESSAGE_HANDLER(WM_NCHITTEST,onNcHitTest)

        TW_MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
        TW_MESSAGE_HANDLER(WM_MOUSELEAVE, onMouseLeave)
        TW_MESSAGE_HANDLER(WM_NCMOUSELEAVE, onMouseLeave)
        TW_MESSAGE_HANDLER(WM_LBUTTONDOWN, onLButtonDown)
        TW_MESSAGE_HANDLER(WM_LBUTTONDBLCLK, onLButtonDBClick)
        TW_MESSAGE_HANDLER(WM_LBUTTONUP, onLButtonUp)
        TW_MESSAGE_HANDLER(WM_RBUTTONDOWN, onRButtonDown)
        TW_MESSAGE_HANDLER(WM_RBUTTONDBLCLK, onRButtonDBClick)
        TW_MESSAGE_HANDLER(WM_RBUTTONUP, onRButtonUp)
        TW_MESSAGE_HANDLER(WM_MBUTTONDOWN, onMButtonDown)
        TW_MESSAGE_HANDLER(WM_MBUTTONDBLCLK, onMButtonDBClick)
        TW_MESSAGE_HANDLER(WM_MBUTTONUP, onMButtonUp)
        TW_MESSAGE_HANDLER(WM_CAPTURECHANGED, onMouseCaputureChanged)

        TW_MESSAGE_HANDLER(WM_MOUSEWHEEL, onWheel)

        //Keyboard
        TW_MESSAGE_HANDLER(WM_KEYDOWN, onKeyDown)
        TW_MESSAGE_HANDLER(WM_KEYUP, onKeyUp)
        TW_MESSAGE_HANDLER(WM_SYSKEYDOWN, onKeyDown)
        TW_MESSAGE_HANDLER(WM_SYSKEYUP, onKeyUp)
        TW_MESSAGE_HANDLER(WM_CHAR, onChar)

    TW_MESSAGEMAP_END()

    bool onCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onClose(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res );
    bool onNcDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMoving(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onSize(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onChar(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
   
    bool onMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onLButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onRButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMButtonDBClick(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);
    bool onMButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onMouseCaputureChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    bool onWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& res);

    virtual PCWSTR  ClassName() const;

private:
    bool m_created;
    DWORD m_windowStyle;
    DWORD m_windowStyleEx;
    std::wstring m_windowClass;

    TwWindow* m_twWindow;

    bool m_isMouseOver;
    bool m_isDragging;
    Tw::MouseButton m_pressedButton;
};
