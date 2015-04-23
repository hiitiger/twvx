#include "stable.h"
#include "twbutton.h"

TW_CLASS_IMPLEMENT(TwButton, TwFrameWidget);

TwButton::TwButton(TwWidget* parent /*= nullptr*/)
    : TwFrameWidget(parent)
    , m_activeColor(k_DefActiveColor)
    , m_pressedColor(k_DefPressedColor)
{
    setAcceptHoverEvent(true);
}

TwButton::~TwButton()
{

}

void TwButton::setActiveColor(TwColor color)
{
    m_activeColor = color;
}

void TwButton::setPressedColor(TwColor color)
{
    m_pressedColor = color;
}

void TwButton::paintBackground(TwPainter* painter)
{
    TwColor fillColor;
    TwColor boundColor;
    if (isEnabled())
    {
        switch (frameState())
        {
        case Tw::State_Normal:
            {
                fillColor = backgroundColor();
                boundColor = k_DefBoundColor;
            }
            break;
        case Tw::State_Active:
            {
                fillColor = m_activeColor;
                boundColor = m_activeColor;
                boundColor.setAlpha(255);
            }
            break;
        case Tw::State_Pressed:
            {
                fillColor = m_pressedColor;
                boundColor = m_pressedColor;
                boundColor.setAlpha(255);
            }
            break;
        }
    }
    else
    {
        fillColor = k_DefDisableFillColor;
        boundColor = k_DefBoundColor;
    }
    if (fillColor.alpha() != 0)
    {
        painter->fillRect(localRect(), fillColor);
    }
    if (boundColor.alpha() != 0)
    {
        painter->drawRect(localRect(), boundColor, 1.0f, true);
    }
}
