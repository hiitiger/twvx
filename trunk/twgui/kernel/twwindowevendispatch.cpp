#include "stable.h"
#include "twrootwidget.h"
#include "twwindow.h"
#include "tweventscrack.h"
#include "twwindowevendispatch.h"
#include "twpainter.h"
#include "renderer/twrendersufaced2d.h"


TwWindowEventDispatcher::TwWindowEventDispatcher()
    : TwEventDispatcher()
    , m_twWindow(nullptr)
    , m_dirtyWindow(false)
{
    m_paintTimer.setInterval(16);
    m_paintTimer.sigTimeout().connect(this, &TwWindowEventDispatcher::onPaintTimerTimeOut);
}

TwWindowEventDispatcher::~TwWindowEventDispatcher()
{

}

void TwWindowEventDispatcher::setTwWindow( TwWindow* w)
{
    m_twWindow = w;
}

void TwWindowEventDispatcher::schedulePaint( const TwRect<int>& rc /*= TwRect<int>()*/ )
{
    TW_UNUSED(rc);
    m_dirtyWindow = true;
}

void TwWindowEventDispatcher::onPaintTimerTimeOut(TwObject* sender)
{
    TW_UNUSED(sender);
    m_twWindow->update();
}

void TwWindowEventDispatcher::dispatchEvent( TwEvent* ev )
{
    m_twWindow->processEvent(ev);
    if (ev->accepted())
    {
        return;
    }

    TW_EVENTCRACK_BEGIN(ev)

        TW_EVENTSINK_R(ev, TwCreateEvent, Tw::Ev_Create, this, onCreateEvent)
        TW_EVENTSINK_R(ev, TwDestroyEvent, Tw::Ev_Destroy, this, onDestroyEvent)

        TW_EVENTSINK_R(ev, TwMoveEvent, Tw::Ev_Move, this, onMoveEvent)
        TW_EVENTSINK_R(ev, TwResizeEvent, Tw::Ev_Resize, this, onResizeEvent)

        TW_EVENTSINK_R(ev, TwPaintEvent, Tw::Ev_Paint, this, onPaintEvent)

        TW_EVENTSINK_R(ev, TwHitTestEvent, Tw::Ev_HitTest, this, onHitTestEvent)

        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MousePress, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseRelease, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseDbClick, this, onMouseEvent)
        TW_EVENTSINK_R(ev, TwMouseEvent, Tw::Ev_MouseMove, this, onMouseEvent)

        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverMove, this, onHoverEvent)
        TW_EVENTSINK_R(ev, TwHoverEvent, Tw::Ev_HoverLeave, this, onHoverEvent)
        TW_EVENTSINK_R(ev, TwWheelEvent, Tw::Ev_Wheel, this, onWheelEvent)

        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyPress, this, onKeyEvent)
        TW_EVENTSINK_R(ev, TwKeyEvent, Tw::Ev_KeyRelease, this, onKeyEvent)
        TW_EVENTSINK_R(ev, TwCharEvent, Tw::Ev_Char, this, onCharEvent)

        TW_EVENTSINK_R(ev, TwShowEvent, Tw::Ev_Show, this, onShowEvent)
        TW_EVENTSINK_R(ev, TwHideEvent, Tw::Ev_Hide, this, onHideEvent)
    TW_EVENTCRACK_END()

}

void TwWindowEventDispatcher::onCreateEvent( TwCreateEvent* ev)
{
    ev->accept();
}

void TwWindowEventDispatcher::onDestroyEvent( TwDestroyEvent* ev)
{
    m_paintTimer.stop();
    ev->accept();
}

void TwWindowEventDispatcher::onShowEvent(TwShowEvent* ev)
{
    m_paintTimer.start();
    ev->accept();
}

void TwWindowEventDispatcher::onHideEvent(TwHideEvent* ev)
{
    m_paintTimer.stop();
    ev->accept();
}

void TwWindowEventDispatcher::onMoveEvent( TwMoveEvent* ev )
{
    ev->accept();
}

void TwWindowEventDispatcher::onResizeEvent( TwResizeEvent* ev)
{
    ev->accept();
}

void TwWindowEventDispatcher::onPaintEvent( TwPaintEvent* ev)
{
    TwPainter* painter = ev->painter();
    if (ev->reason() == TwPaintEvent::NcPaint)
    {
        ev->reject();
        return;
    }
    if (ev->reason() == TwPaintEvent::EraseBackground)
    {
        ev->accept();  
        return;
    }
    if (ev->reason() == TwPaintEvent::ContentPaint)
    {
        painter->beginRender();

        TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();

        painter->saveState();
        painter->pushClipRect(rootWidget->boundRect());
        painter->translate(rootWidget->pos());
        rootWidget->paintWidgetAndChildren(painter);

        painter->restoreState();
        painter->endRender();

        ev->accept();
    }

}

void TwWindowEventDispatcher::onHitTestEvent( TwHitTestEvent* ev)
{
    //if in window boarder, use default handling.
    TwPoint<int > clientPoint = m_twWindow->mapFromWindowToClient(ev->m_windowPoint);
    if (m_twWindow->clientRect().contains(clientPoint))
    {
        ev->m_localPoint = clientPoint;
        TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
        rootWidget->dispatchHitTestEvent(ev);
    }
}

void TwWindowEventDispatcher::onMouseEvent( TwMouseEvent* ev)
{
    ev->m_localPos = ev->clientPos();
    TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
    rootWidget->dispatchMouseEvent(ev);
}

void TwWindowEventDispatcher::onHoverEvent(TwHoverEvent* ev)
{
    ev->m_localPos = ev->clientPos();
    TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
    rootWidget->disPatchHoverEvent(ev);
}

void TwWindowEventDispatcher::onWheelEvent(TwWheelEvent* ev)
{
    ev->m_localPos = ev->clientPos();
    TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
    rootWidget->disPatachWheelEvent(ev);
}

void TwWindowEventDispatcher::onKeyEvent( TwKeyEvent* ev)
{
    TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
    rootWidget->dispatchKeyEvent(ev);
}

void TwWindowEventDispatcher::onCharEvent( TwCharEvent* ev)
{
    TwRootWidget* rootWidget = m_twWindow->m_rootWidget.get();
    rootWidget->dispatchCharEvent(ev);
}

