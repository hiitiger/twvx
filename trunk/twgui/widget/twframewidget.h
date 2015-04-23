#pragma once

class TW_GUI_API TwFrameWidget : public TwWidget
{
    TW_CLASS_DECLARE(TwFrameWidget);
public:

    explicit TwFrameWidget(TwWidget* parent = nullptr);
    ~TwFrameWidget();

    Tw::FrameState frameState() const { return m_frameState; }

    void setPressed(bool);
    bool isPressed() const { return m_frameState == Tw::State_Pressed; }
    void setActive(bool);
    bool isActive() const { return m_frameState >= Tw::State_Active; }

public: //signals:
    TwSignal<void()> sigClicked;
    TwSignal<void()> sigPressed;
    TwSignal<void()> sigReleased;

protected:
    virtual void onEnableChanged();

    virtual void onFrameStateChanged(Tw::FrameState oldState);

    virtual void onMousePresseEvent(TwMouseEvent* ev);
    virtual void onMouseReleaseEvent(TwMouseEvent* ev);
    virtual void onMouseMoveEvent(TwMouseEvent* ev);

    virtual void onHoverEnterEvent(TwHoverEvent* ev);
    virtual void onHoverLeaveEvent(TwHoverEvent* ev);

private:
    Tw::FrameState m_frameState;
};

TW_CLASS_REGISTER(TwFrameWidget);
