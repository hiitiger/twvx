#pragma once

#include "twevent.h"

class TW_GUI_API TwInputEvent : public TwEvent
{
public:
    explicit TwInputEvent(Tw::EventType id)
        : TwEvent(id)
    {
        ;
    }
};

class TW_GUI_API TwMouseEvent : public TwInputEvent
{
public:
    explicit TwMouseEvent(Tw::EventType id)
        :TwInputEvent(id)
    {

    }

    TwMouseEvent(Tw::EventType id, const TwPoint<int>& screenPos, const TwPoint<int>& clientPos, const TwPoint<int>& localPos, Tw::MouseButton button = Tw::MB_NoButton)
        : TwInputEvent(id)
        , m_screenPos(screenPos)
        , m_clientPos(clientPos)
        , m_localPos(localPos)
        , m_button(button)
    {
    }

    TwPoint<int> clientPos() const { return m_clientPos; }
    TwPoint<int> screenPos() const { return m_screenPos; }
    TwPoint<int> localPos() const { return m_localPos; }
    Tw::MouseButton button() const { return m_button; }

    TwPoint<int> m_screenPos;
    TwPoint<int> m_clientPos;
    TwPoint<int> m_localPos;
    Tw::MouseButton m_button;
};

class TW_GUI_API TwHoverEvent : public TwInputEvent
{
public:
    explicit TwHoverEvent(Tw::EventType id)
        :TwInputEvent(id)
    {

    }

    TwHoverEvent(Tw::EventType id, const TwPoint<int>& screenPos, const TwPoint<int>& clientPos, const TwPoint<int>& localPos)
        : TwInputEvent(id)
        , m_screenPos(screenPos)
        , m_clientPos(clientPos)
        , m_localPos(localPos)
    {
    }

    TwPoint<int> clientPos() const { return m_clientPos; }
    TwPoint<int> screenPos() const { return m_screenPos; }
    TwPoint<int> localPos() const { return m_localPos; }

    TwPoint<int> m_screenPos;
    TwPoint<int> m_clientPos;
    TwPoint<int> m_localPos;
};

class TW_GUI_API TwWheelEvent : public TwInputEvent
{
public:
    TwWheelEvent(int offset, const TwPoint<int>& screenPos, const TwPoint<int>& clientPos, const TwPoint<int>& localPos)
        : TwInputEvent(Tw::Ev_Wheel)
        , m_offset(offset)
        , m_screenPos(screenPos)
        , m_clientPos(clientPos)
        , m_localPos(localPos)
    {
    }

    static const int wheeldelta = 120;

    int offset() const { return m_offset; }

    TwPoint<int> clientPos() const { return m_clientPos; }
    TwPoint<int> screenPos() const { return m_screenPos; }
    TwPoint<int> localPos() const { return m_localPos; }

    int m_offset;
    TwPoint<int> m_screenPos;
    TwPoint<int> m_clientPos;
    TwPoint<int> m_localPos;
};

class TW_GUI_API TwDragEvent : public TwEvent
{
public:
    TwDragEvent(Tw::EventType id, const TwPoint<int>& screenPos, const TwPoint<int>& clientPos, const TwPoint<int>& localPos)
        : TwEvent(id)
        , m_screenPos(screenPos)
        , m_clientPos(clientPos)
        , m_localPos(localPos)
    {
    }

    TwPoint<int> clientPos() const { return m_clientPos; }
    TwPoint<int> screenPos() const { return m_screenPos; }
    TwPoint<int> localPos() const { return m_localPos; }

    TwPoint<int> m_screenPos;
    TwPoint<int> m_clientPos;
    TwPoint<int> m_localPos;
};

class TW_GUI_API TwHitTestEvent : public TwEvent
{
public:
    enum Result
    {
        Border,
        Top,
        Bottom,
        Left,
        Right,
        BottomLeft,
        BottomRight,
        TopLeft,
        TopRight,
        Caption,
        Client,
        MinimumButton,
        MaximumButton,
        CloseButton,
        SysMenu,
        NoResult,
    };
    TwHitTestEvent(const TwPoint<int>& windowPoint)
        : TwEvent(Tw::Ev_HitTest)
        , m_windowPoint(windowPoint)
        , m_localPoint(windowPoint)
        , m_result(NoResult)
    {
        ;
    }

    Result result() const { return m_result; }
    void setResult(Result r) { m_result = r; }
    TwPoint<int> localPos() const { return m_localPoint; }


    Result m_result;
    TwPoint<int> m_windowPoint;
    TwPoint<int> m_localPoint;
};

class TW_GUI_API TwKeyEvent : public TwInputEvent
{
public:
    explicit TwKeyEvent(Tw::EventType id)
        : TwInputEvent(id)
    {

    }

    int m_key;
    bool m_ctrl;
    bool m_shift;
    bool m_alt;
    bool m_capslock;
};

class TW_GUI_API TwCharEvent : public TwInputEvent
{
public:
    TwCharEvent()
        : TwInputEvent(Tw::Ev_Char)
    {

    }

    wchar_t m_char;
    bool m_ctrl;
    bool m_shift;
    bool m_alt;
    bool m_capslock;
};

class TW_GUI_API TwSysCmdEvent : public TwEvent
{
public:
    explicit TwSysCmdEvent(Tw::EventType id)
        : TwEvent(id)
    {
        ;
    }
    ~TwSysCmdEvent() { ; }
};

class TW_GUI_API TwCloseEvent : public TwSysCmdEvent
{
public:
    TwCloseEvent()
        : TwSysCmdEvent(Tw::Ev_Close)
    {
        ;
    }
};

class TW_GUI_API TwCreateEvent : public TwEvent
{
public:
    TwCreateEvent()
        : TwEvent(Tw::Ev_Create)
    {
        ;
    }

};

class TW_GUI_API TwDestroyEvent : public TwEvent
{
public:
    TwDestroyEvent()
        : TwEvent(Tw::Ev_Destroy)
    {
        ;
    }

};

class TW_GUI_API TwShowEvent : public TwEvent
{
public:
    TwShowEvent()
        : TwEvent(Tw::Ev_Show)
    {
        ;
    }

};

class TW_GUI_API TwHideEvent : public TwEvent
{
public:
    TwHideEvent()
        : TwEvent(Tw::Ev_Hide)
    {
        ;
    }

};

class TW_GUI_API TwMovingEvent : public TwEvent
{
public:
    TwMovingEvent(const TwRect<int>& preferredRect)
        : TwEvent(Tw::Ev_Moving)
        , m_preferredRect(preferredRect)
    {
        ;
    }

    void setPreferredRect(const TwRect<int>& rc) { m_preferredRect = rc; }

    TwRect<int> preferredRect() const { return m_preferredRect; }

    TwRect<int> m_preferredRect;
};

class TW_GUI_API TwResizeEvent : public TwEvent
{
public:
    TwResizeEvent(const TwSize<int>& newSize, const TwSize<int>& oldSize)
        : TwEvent(Tw::Ev_Resize)
        , m_newSize(newSize)
        , m_oldSize(oldSize)
    {
        ;
    }

    TwSize<int> newSize() const { return m_newSize; }
    TwSize<int> oldSize() const { return m_oldSize; }

    TwSize<int> m_newSize;
    TwSize<int> m_oldSize;

};

class TW_GUI_API TwMoveEvent : public TwEvent
{
public:
    TwMoveEvent(const TwPoint<int>& newpos, const TwPoint<int>& oldpos)
        : TwEvent(Tw::Ev_Move)
        , m_newPos(newpos)
        , m_oldPos(oldpos)
    {
        ;
    }

    TwPoint<int> newPos() const { return m_newPos; }
    TwPoint<int> oldPos() const { return m_oldPos; }

    TwPoint<int> m_newPos;
    TwPoint<int> m_oldPos;

};

class TwPainter;
class TW_GUI_API TwPaintEvent : public TwEvent
{
public:
    enum Reason
    {
        ContentPaint,
        NcPaint,
        EraseBackground
    };
    TwPaintEvent(Reason r, TwPainter* painter, const TwRect<int>& invalidRect = TwRect<int>())
        : TwEvent(Tw::Ev_Paint)
        , m_reason(r)
        , m_painter(painter)
        , m_invalidRect(invalidRect)
    {
        ;
    }

    Reason reason() const { return m_reason; }

    TwPainter* painter() const { return m_painter; }

    Reason m_reason;
    TwPainter* m_painter;
    TwRect<int> m_invalidRect;
};

class TW_GUI_API TwFocusEvent : public TwEvent
{
public:
    TwFocusEvent(Tw::EventType id)
        :TwEvent(id)
    {

    }

    bool isFocusIn() const
    {
        return TwEvent::id() == Tw::Ev_FocusIn;
    }

    bool isFocusOut() const
    {
        return TwEvent::id() == Tw::Ev_FocusOut;
    }
};