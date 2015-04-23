#include "stable.h"
#include "twswitchbutton.h"


TW_CLASS_IMPLEMENT(TwSwitchButton, TwButton);

const int k_SliderWidth = 10;

TwSwitchButton::TwSwitchButton(TwWidget* parent /*= nullptr*/)
    : TwButton(parent)
    , sigSwitchStateChanged(this)
    , m_switchState(Tw::Off)
    , m_sliderRect(0,0,k_SliderWidth,20)
{
    sigClicked.connect(this, &TwSwitchButton::onClicked);
}

TwSwitchButton::~TwSwitchButton()
{

}

void TwSwitchButton::setSwitchState(Tw::SwitchState s)
{
    if (m_switchState != s)
    {
        m_switchState = s;
        onSwitchStateChanged();
        update();

        sigSwitchStateChanged.emit(m_switchState);
    }
}

TwSize<int> TwSwitchButton::hintSize() const
{
    return TwSize<int>(50, 20);
}

void TwSwitchButton::paintBackground(TwPainter*)
{

}

void TwSwitchButton::paint(TwPainter* painter)
{
    painter->drawRect(localRect(), k_DefBoundColor, 4.0f);

    if (isSwitchedOn())
    {
        painter->fillRect(localRect().adjust(4), TwColor::twColor(TwColor::LightSkyBlue));
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Dark));
    }
    else
    {
        painter->fillRect(localRect().adjust(4), TwColor::twColor(TwColor::SilverChalice));
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Grey));
    }

}

void TwSwitchButton::onClicked(TwObject*)
{
    if (isSwitchedOn())
    {
        setSwitchState(Tw::Off);
    }
    else
    {
        setSwitchState(Tw::On);
    }
}

void TwSwitchButton::onSwitchStateChanged()
{
    updateSliderRect();
}

void TwSwitchButton::updateSliderRect()
{
    if (isSwitchedOn())
    {
        m_sliderRect.setLeft(width() - k_SliderWidth);
    }
    else
    {
        m_sliderRect.setLeft(0);
    }
}

void TwSwitchButton::onResizeEvent(TwResizeEvent* ev)
{
    updateSliderRect();
    m_sliderRect.setHeight(ev->newSize().height());
}


