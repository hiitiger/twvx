#include "stable.h"

#include "windowshelper.h"


void TwCursor::setCursor(TwCursor cursor)
{
    setCursor(cursor.cursorType());
}

void TwCursor::setCursor(CursorType type)
{
    if (WindowsCursorS::windowsCursors()->m_currentCursor != WindowsCursorS::windowsCursors()->getWindowsCursor(type))
    {
        WindowsCursorS::windowsCursors()->m_currentCursor = WindowsCursorS::windowsCursors()->getWindowsCursor(type);
        ::SetCursor(WindowsCursorS::windowsCursors()->m_currentCursor->cursorHandle());
    }
}

void TwCursor::saveCursor()
{
    WindowsCursorS::windowsCursors()->m_cursorQueue.push_back(WindowsCursorS::windowsCursors()->m_currentCursor);
}

void TwCursor::restoreCursor()
{
    if (WindowsCursorS::windowsCursors()->m_cursorQueue.size() > 0)
    {
        Windows::WindowsCursor* cursor = WindowsCursorS::windowsCursors()->m_cursorQueue.back();
        WindowsCursorS::windowsCursors()->m_cursorQueue.pop_back();
        WindowsCursorS::windowsCursors()->m_currentCursor = cursor;
        ::SetCursor(WindowsCursorS::windowsCursors()->m_currentCursor->cursorHandle());
    }
}
