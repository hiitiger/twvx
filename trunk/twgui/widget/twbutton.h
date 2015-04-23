#pragma once

class TW_GUI_API TwButton : public TwFrameWidget
{
    TW_CLASS_DECLARE(TwButton);
public:

    explicit TwButton(TwWidget* parent = nullptr);
    ~TwButton();

    void setActiveColor(TwColor color);
    void setPressedColor(TwColor color);

    virtual void paintBackground(TwPainter*);

protected:
    virtual void onFrameStateChanged(Tw::FrameState oldState) { TW_UNUSED(oldState); }

private:

    TwColor m_activeColor;
    TwColor m_pressedColor;
};

TW_CLASS_REGISTER(TwButton);
