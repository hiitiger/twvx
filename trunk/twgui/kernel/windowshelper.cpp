#pragma once
#include "windowshelper.h"

namespace Windows
{

    bool isWindow(HWND hwnd)
    {
        return ::IsWindow(hwnd) == TRUE;
    }

    bool isKeyDown(int vkey)
    {
        return !!(::GetKeyState(vkey) & 0XF0);
    }

    bool isKeyToggoled(int vkey)
    {
        return !!(::GetKeyState(vkey) & 0X0F);
    }

    bool isCapsLock()
    {
        return isKeyToggoled(VK_CAPITAL);
    }

    WindowsCursor::WindowsCursor(TwCursor::CursorType type)
        : m_handle(nullptr)
        , m_type(type)
    {
        wchar_t* id = nullptr;
        switch (type)
        {
        case TwCursor::ArrowCursor:
            id = IDC_ARROW;
            break;
        case TwCursor::WaitCursor:
            id = IDC_WAIT;
            break;
        case TwCursor::CrossCursor:
            id = IDC_CROSS;
            break;
        case TwCursor::IBeamCursor:
            id = IDC_IBEAM;
            break;
        case TwCursor::HandCursor:
            id = IDC_HAND;
            break;
        case TwCursor::HelpCursor:
            id = IDC_HELP;
            break;
        default:
            break;
        }

        m_handle = (HCURSOR)::LoadImageW(NULL, id, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
    }

    WindowsCursor::~WindowsCursor()
    {
        DestroyCursor(m_handle);
    }

} //Windows

TwDefine_SingleTon(WindowsCursorS, _windowsCursors);

WindowsCursorS* WindowsCursorS::windowsCursors()
{
    return _windowsCursors();
}
