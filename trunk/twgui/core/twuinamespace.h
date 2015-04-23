#pragma once

namespace Tw
{
    enum EventType
    {
        Ev_None         = 0,
        Ev_AppService   = 1,

        Ev_HitTest      = 9,

        Ev_Input        = 10,
        Ev_MousePress   = 11,
        Ev_MouseRelease = 12,
        Ev_MouseDbClick = 13,
        Ev_MouseMove    = 14,

        Ev_HoverMove    = 15,
        Ev_HoverEnter   = 16,
        Ev_HoverLeave   = 17,
        Ev_Wheel        = 18,

        Ev_KeyPress     = 20,
        Ev_KeyRelease   = 21,
        Ev_Char         = 22,

        Ev_FocusIn      = 23,
        Ev_FocusOut     = 24,

        Ev_Create       = 25,
        Ev_Destroy      = 26,
        Ev_Show         = 27,
        Ev_Hide         = 28,
        Ev_Close        = 29,
        Ev_Minimize     = 30,
        Ev_Maximize     = 31,
        Ev_Resize       = 32,
        Ev_Move         = 33,
        Ev_Moving       = 34,
        Ev_Paint        = 35,

        Ev_DragMove     = 40,
        Ev_DragDrop     = 41,

        Ev_LayoutRequest = 100,

        Ev_User         = 1000,

        Ev_UserMax      = 1000+10000,

    };

    enum MouseButton
    {
        MB_NoButton    = 0,
        MB_LeftButton  = 0x1,
        MB_RightButton = 0x2,
        MB_MidButton   = 0x4
    };

    enum WindowAttribute
    {
        WA_DeleteOnWindowDestroyed = 0x1,
    };

    enum WidgetFlag
    {
        AcceptHoverEvent    = 0x1,
        AcceptDragMoveEvent = 0x2,
        AcceptDragDropEvent = 0x4,

        HasChildren         = 0x00010000
    };


    enum SizePolicy
    {
        Preferred,
        Fixed
    };

    enum Alignment
    {
        Align_Stretch = 0x0,
        Align_Left    = 0x1,
        Align_Right   = 0x2,
        Align_Center  = 0x4,
        Align_Top     = 0x8,
        Align_Bottom  = 0x10,
    };

    enum  Orientation
    {
        Horizontal,
        Vertical
    };

    enum SwitchState
    {
        Off,
        On
    };

    enum Direction
    {
        NoneDirection = 0x0,
        Left          = 0x1,
        Right         = 0x2,
        Up            = 0x4,
        Down          = 0x8
    };

    enum FrameState
    {
        State_Normal,
        State_Active,
        State_Pressed
    };

}

