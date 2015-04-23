#include "stable.h"
#include "twslider.h"


TW_CLASS_IMPLEMENT(TwSlider, TwWidget);

const int k_SliderWidth = 10;

TwSlider::TwSlider(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , sigValueChanged(this)
    , m_minValue(0.f)
    , m_maxValue(1.f)
    , m_currentValue(0.f)
    , m_sliderRect(0,0,10,20)
    , m_dragStartValue(0.f)
    , m_isDragSlider(false)
    , m_hovered(false)
{
    setAcceptHoverEvent(true);
}

TwSlider::~TwSlider()
{

}

void TwSlider::setCurrentValue(float value)
{
    if (value < m_minValue)
    {
        value = m_minValue;
    }
    if (value > m_maxValue)
    {
        value = m_maxValue;
    }

    if (!twCompareValue(m_currentValue, value))
    {
        float oldValue = m_currentValue;
        m_currentValue = value;
        onCurrentValueChanged(oldValue);

        sigValueChanged.emit(m_currentValue);
    }
}

void TwSlider::setRange(float min, float max)
{
    m_minValue = min;
    m_maxValue = max;
}

TwSize<int> TwSlider::hintSize() const
{
    return TwSize<int>(100, 20);
}

void TwSlider::paintBackground(TwPainter* painter)
{
    painter->drawRect(localRect(), k_DefBoundColor, 1.0f, true);

    TwRect<int> rc = localRect();
    if (m_hovered)
    {
        painter->fillRect(rc.adjust(1), TwColor::twColor(TwColor::LightBlue));
    }
    else
    {
        painter->fillRect(rc.adjust(1), TwColor::twColor(TwColor::LightSkyBlue));
    }
}

void TwSlider::paint(TwPainter* painter)
{
    if (m_isDragSlider)
    {
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Dark));
    }
    else
    {
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Grey51));
    }
}

void TwSlider::onCurrentValueChanged(float oldValue)
{
    TW_UNUSED(oldValue);
    m_sliderRect.setLeft(static_cast<int>(currentValue() * ( width() - k_SliderWidth )));

    update();
}

void TwSlider::onResizeEvent(TwResizeEvent* ev)
{
    TwWidget::onResizeEvent(ev);
    m_sliderRect.setLeft(static_cast<int>(currentValue() * (width() - k_SliderWidth)));
}

void TwSlider::onMousePresseEvent(TwMouseEvent* ev)
{
    if (m_sliderRect.contains(ev->localPos()))
    {
        ev->accept();
        m_isDragSlider = true;
        m_dragStartPoint = ev->localPos();
        m_dragStartValue = currentValue();

        update();
    }
}

void TwSlider::onMouseReleaseEvent(TwMouseEvent* ev)
{
    TW_UNUSED(ev);
    m_isDragSlider = false;
    update();
}

void TwSlider::onMouseMoveEvent(TwMouseEvent* ev)
{
    if (m_isDragSlider)
    {
        TwPoint<int> delta = ev->localPos() - m_dragStartPoint;
        setCurrentValue(m_dragStartValue + range() * delta.x() / (width() - k_SliderWidth));
        update();
    }
}

void TwSlider::onHoverEnterEvent(TwHoverEvent* ev)
{
    TW_UNUSED(ev);
    m_hovered = true;
}

void TwSlider::onHoverLeaveEvent(TwHoverEvent* ev)
{
    TW_UNUSED(ev);
    m_hovered = false;
}
