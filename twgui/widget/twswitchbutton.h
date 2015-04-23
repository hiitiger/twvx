#pragma once

class TW_GUI_API TwSwitchButton : public TwButton
{
    TW_CLASS_DECLARE(TwSwitchButton);
public:
    explicit TwSwitchButton(TwWidget* parent = nullptr);
    ~TwSwitchButton();

    void setSwitchState(Tw::SwitchState s);
    Tw::SwitchState switchState() const { return m_switchState; }

    void switchOn() { setSwitchState(Tw::On); }
    void switchOff() { setSwitchState(Tw::Off); }

    bool isSwitchedOn() const { return m_switchState == Tw::On; }

    virtual TwSize<int> hintSize() const;

    virtual void paintBackground(TwPainter*);
    virtual void paint(TwPainter*);

public:
    TwSignal<void(Tw::SwitchState)> sigSwitchStateChanged;

protected:
    void onClicked(TwObject*);

    virtual void onSwitchStateChanged();
    virtual void updateSliderRect();

    virtual void onResizeEvent(TwResizeEvent* ev);

private:
    Tw::SwitchState m_switchState;
    TwRect<int> m_sliderRect;
};

TW_CLASS_REGISTER(TwSwitchButton);
