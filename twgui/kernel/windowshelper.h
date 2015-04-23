#pragma once

namespace Windows
{
    bool isWindow(HWND hwnd);

    bool isKeyDown(int vkey);
    bool isKeyToggoled(int vkey);
    bool isCapsLock();

    class WindowsCursor 
    {
    public:
        WindowsCursor(TwCursor::CursorType type);
        ~WindowsCursor();

        HCURSOR cursorHandle() { return m_handle; }
        TwCursor::CursorType cursorType() const { return m_type; }

    private:
        HCURSOR m_handle;
        TwCursor::CursorType m_type;
    };
}

class WindowsCursorS
{
public:
    WindowsCursorS()
    {
        m_windowsCursors.reserve(TwCursor::SysCursorCount);
        for (int i = 0; i != (int)TwCursor::SysCursorCount; ++i)
        {
            m_windowsCursors.push_back(std::shared_ptr<Windows::WindowsCursor>(new Windows::WindowsCursor((TwCursor::CursorType)i)));
        }
        m_currentCursor = getWindowsCursor(TwCursor::ArrowCursor);
    }

    Windows::WindowsCursor* getWindowsCursor(TwCursor::CursorType type)
    {
        if ((size_t)type < m_windowsCursors.size() && (int)type >= 0)
        {
            return m_windowsCursors[(int)type].get();
        }
        else
        {
            DAssert(false);
            return nullptr;
        }
    }

    static WindowsCursorS* windowsCursors();

    std::vector<std::shared_ptr<Windows::WindowsCursor>> m_windowsCursors;
    std::deque<Windows::WindowsCursor*> m_cursorQueue;
    Windows::WindowsCursor* m_currentCursor;
};


