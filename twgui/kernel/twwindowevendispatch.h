#pragma once

#include "tweventdispatch.h"

class TW_GUI_API TwWindowEventDispatcher : public TwEventDispatcher
{
public:
    TwWindowEventDispatcher();
    virtual ~TwWindowEventDispatcher();
    void setTwWindow(TwWindow*);
    virtual void dispatchEvent(TwEvent* ev);

    void schedulePaint(const TwRect<int>& rc = TwRect<int>());
    void onPaintTimerTimeOut(TwObject* sender);

protected:

    virtual void onCreateEvent(TwCreateEvent*);
    virtual void onDestroyEvent(TwDestroyEvent*);

    virtual void onShowEvent(TwShowEvent*);
    virtual void onHideEvent(TwHideEvent*);

    virtual void onMoveEvent(TwMoveEvent* ev);
    virtual void onResizeEvent(TwResizeEvent*);
    virtual void onPaintEvent(TwPaintEvent*);

    virtual void onHitTestEvent(TwHitTestEvent*);

    virtual void onMouseEvent(TwMouseEvent*);

    virtual void onHoverEvent(TwHoverEvent*);
    virtual void onWheelEvent(TwWheelEvent*);

    virtual void onKeyEvent(TwKeyEvent*);
    virtual void onCharEvent(TwCharEvent*);


protected:
    TwWindow* m_twWindow;

    bool m_dirtyWindow;
    TwTimer m_paintTimer;
};