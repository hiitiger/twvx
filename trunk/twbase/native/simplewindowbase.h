#pragma once

template <class DERIVED_TYPE> 
class SimpleWindowBase
{
public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        DERIVED_TYPE *pThis = nullptr;

        if (uMsg == WM_NCCREATE)
        {
            CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
            pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

            pThis->m_hwnd = hwnd;
        }
        else
        {
            pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (pThis)
        {
            return pThis->HandleMessage(uMsg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    SimpleWindowBase()
        : m_hwnd(nullptr)
    {
        ;
    }

    virtual ~SimpleWindowBase()
    {
        
    }

    void RegisterWindowClass()
    {
        WNDCLASSEX wc = {0};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc   = SimpleWindowBase::WindowProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = ClassName();
        wc.hCursor		 = LoadCursor(nullptr, IDC_ARROW);

        ATOM r = RegisterClassEx(&wc);
        TW_UNUSED(r);
        DAssert_M(r, "registerClass");
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = CW_USEDEFAULT,
        int nHeight = CW_USEDEFAULT,
        HWND hWndParent = nullptr,
        HMENU hMenu = nullptr
        )
    {
        m_hwnd = CreateWindowEx(
            dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
            nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(nullptr), this
            );

        return (m_hwnd ? TRUE : FALSE);
    }
    void DestroyWindow() 
    { 
        if (::IsWindow(m_hwnd))
        {
            ::DestroyWindow(m_hwnd);
        }
        m_hwnd = nullptr;
    }

    HWND Window() const { return m_hwnd; }

    LRESULT SendMessage(UINT m,WPARAM w,LPARAM l) {return ::SendMessage(m_hwnd,m,w,l);}
    LRESULT PostMessage(UINT m,WPARAM w,LPARAM l) {return ::PostMessage(m_hwnd,m,w,l);}

protected:

    virtual PCWSTR  ClassName() const = 0;
 
    HWND m_hwnd;
};
