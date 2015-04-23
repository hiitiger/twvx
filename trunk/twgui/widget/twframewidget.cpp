#include "stable.h"
#include "twframewidget.h"

TW_CLASS_IMPLEMENT(TwFrameWidget, TwWidget);

TwFrameWidget::TwFrameWidget(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , sigClicked(this)
    , sigPressed(this)
    , sigReleased(this)
    , m_frameState(Tw::State_Normal)
{
    setAcceptHoverEvent(true);
}

TwFrameWidget::~TwFrameWidget()
{

}
void TwFrameWidget::setPressed(bool pressed)
{
    if (pressed)
    {
        if (m_frameState < Tw::State_Pressed)
        {
            Tw::FrameState oldState = m_frameState;
            m_frameState = Tw::State_Pressed;
            onFrameStateChanged(oldState);
        }
    }
    else
    {
        if (m_frameState == Tw::State_Pressed)
        {
            Tw::FrameState oldState = m_frameState;
            m_frameState = Tw::State_Normal;
            onFrameStateChanged(oldState);
        }
    }
}

void TwFrameWidget::setActive(bool active)
{
    if (active)
    {
        if (m_frameState < Tw::State_Active)
        {
            Tw::FrameState oldState = m_frameState;
            m_frameState = Tw::State_Active;
            onFrameStateChanged(oldState);
        }
    }
    else
    {
        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Normal;
        onFrameStateChanged(oldState);
    }
}

void TwFrameWidget::onEnableChanged()
{
    Tw::FrameState oldState = m_frameState;
    m_frameState = Tw::State_Normal;
    onFrameStateChanged(oldState);

    TwWidget::onEnableChanged();
}

void TwFrameWidget::onFrameStateChanged(Tw::FrameState oldState)
{
    TW_UNUSED(oldState);
}

void TwFrameWidget::onMousePresseEvent(TwMouseEvent* ev)
{
    TwWidget::onMousePresseEvent(ev);
    ev->accept();

    Tw::FrameState oldState = m_frameState;
    m_frameState = Tw::State_Pressed;
    onFrameStateChanged(oldState);

    sigPressed.emit();

    update();
}

void TwFrameWidget::onMouseReleaseEvent(TwMouseEvent* ev)
{
    TwWidget::onMouseReleaseEvent(ev);

    sigReleased.emit();

    if (m_frameState == Tw::State_Pressed && localRect().contains(ev->localPos()))
    {
        ev->accept();

        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Active;
        onFrameStateChanged(oldState);

        sigClicked.emit();
    }
    else
    {
        m_frameState = Tw::State_Normal;
    }

    update();
}

void TwFrameWidget::onMouseMoveEvent(TwMouseEvent* ev)
{
    if (contains(ev->localPos()))
    {
        if (m_frameState == Tw::State_Active)
        {
            Tw::FrameState oldState = m_frameState;
            m_frameState = Tw::State_Pressed;
            onFrameStateChanged(oldState);
            update();
        }
    }
    else
    {
        if (m_frameState == Tw::State_Pressed)
        {
            Tw::FrameState oldState = m_frameState;
            m_frameState = Tw::State_Active;
            onFrameStateChanged(oldState);
            update();
        }
    }
}

void TwFrameWidget::onHoverEnterEvent(TwHoverEvent* ev)
{
    TwWidget::onHoverEnterEvent(ev);

    if (m_frameState == Tw::State_Normal)
    {
        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Active;
        onFrameStateChanged(oldState);
    }
    else if (m_frameState == Tw::State_Active)
    {
        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Pressed;
        onFrameStateChanged(oldState);
    }
    update();
}

void TwFrameWidget::onHoverLeaveEvent(TwHoverEvent* ev)
{
    TwWidget::onHoverLeaveEvent(ev);

    if (m_frameState == Tw::State_Active)
    {
        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Normal;
        onFrameStateChanged(oldState);
    }
    else if (m_frameState == Tw::State_Pressed)
    {
        Tw::FrameState oldState = m_frameState;
        m_frameState = Tw::State_Active;
        onFrameStateChanged(oldState);
    }
    update();
}

