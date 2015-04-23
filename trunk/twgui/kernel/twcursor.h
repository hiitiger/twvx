#pragma once

class TW_GUI_API TwCursor
{
public:
    enum CursorType
    {
        ArrowCursor,
        WaitCursor,
        CrossCursor,
        IBeamCursor,
        HandCursor,
        HelpCursor,
        SysCursorCount,
    };

    TwCursor()
        : m_type(ArrowCursor)
    {

    }

    TwCursor(CursorType type)
        : m_type(type)
    {

    }

    ~TwCursor()
    {

    }

    CursorType cursorType() { return m_type; }

    static void setCursor(TwCursor cursor);
    static void setCursor(CursorType type);
    static void saveCursor();
    static void restoreCursor();

private:
    CursorType m_type;
};