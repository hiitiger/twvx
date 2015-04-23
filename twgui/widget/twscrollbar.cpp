#include "stable.h"
#include "twscrollbar.h"


TW_CLASS_IMPLEMENT(TwScrollBar, TwWidget);

const int k_ScrollX = 20;
const int k_ScrollY = 10;
const int k_ScrollW = 50;

TwScrollBar::TwScrollBar(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , sigScrollPostionChanged(this)
    , sigPressed(this)
    , m_minButton(nullptr)
    , m_maxButton(nullptr)
    , m_orientation(Tw::Vertical)
    , m_minPostion(0.f)
    , m_maxPostion(0.f)
    , m_currentPostion(0.f)
    , m_pageStep(0.f)
    , m_singleStep(20.f)
    , m_dragStartPostion(0.f)
    , m_isDragSlider(false)
    , m_hovered(false)
    , m_autoScrollTimer(this)
    , m_autoDirection(Tw::NoneDirection)
    , m_autoModeSingleStep(true)
    , m_autoHide(false)
{
    setAcceptHoverEvent(true);

    m_minButton = new TwButton(this);
    m_minButton->setBackgroundColor(k_DefMiddleFillColor);
    m_minButton->setActiveColor(k_DefFillColor);
    m_minButton->setPressedColor(k_DefDarkFillColor);

    m_minButton->sigClicked.connect(this, &TwScrollBar::onMinButtonClicked);
    m_minButton->sigPressed.connect(this, &TwScrollBar::onMinButtonPressed);
    m_minButton->sigReleased.connect(this, &TwScrollBar::onMinButtonReleased);

    m_maxButton = new TwButton(this);
    m_maxButton->setBackgroundColor(k_DefMiddleFillColor);
    m_maxButton->setActiveColor(k_DefFillColor);
    m_maxButton->setPressedColor(k_DefDarkFillColor);
    m_maxButton->sigClicked.connect(this, &TwScrollBar::onMaxButtonClicked);
    m_maxButton->sigPressed.connect(this, &TwScrollBar::onMaxButtonPressed);
    m_maxButton->sigReleased.connect(this, &TwScrollBar::onMaxButtonReleased);

    m_autoScrollTimer.setElapse(160);
    m_autoScrollTimer.sigTimeOut.connect(this, &TwScrollBar::onAutoScrollTimerUpdate);
}

TwScrollBar::~TwScrollBar()
{

}

void TwScrollBar::setRange(float min, float max)
{
    m_minPostion = min;
    m_maxPostion = max;
    updateSlider();
}

void TwScrollBar::setPageStep(float step)
{
    if (m_pageStep != step)
    {
        m_pageStep = step;
        updateSlider();
    }
}

void TwScrollBar::setSingleStep(float step)
{
    m_singleStep = step;
}

void TwScrollBar::scrollToPostion(float postion)
{
    if (postion < m_minPostion)
    {
        postion = m_minPostion;
    }
    if (postion > m_maxPostion)
    {
        postion = m_maxPostion;
    }

    if (!twCompareValue(m_currentPostion, postion))
    {
        float oldPostion = m_currentPostion;
        m_currentPostion = postion;
        onPostionChanged(oldPostion);

        sigScrollPostionChanged.emit(m_currentPostion);
    }
}

void TwScrollBar::scroll(float step)
{
    if (step > 0)
    {
        scrollDown(step);
    }
    else if (step < 0)
    {
        scrollUp(-step);
    }
}

void TwScrollBar::scrollUp(float step)
{
    scrollToPostion(m_currentPostion - step);
}

void TwScrollBar::scrollDown(float step)
{
    scrollToPostion(m_currentPostion + step);
}

void TwScrollBar::scrollToMin()
{
    scrollToPostion(m_minPostion);
}

void TwScrollBar::scrollToMax()
{
    scrollToPostion(m_maxPostion);
}

void TwScrollBar::setOrientation(Tw::Orientation o)
{
    if (m_orientation != o)
    {
        m_orientation = o;
        updateSlider();
    }
}

void TwScrollBar::setAutoHide(bool autoHide)
{
    if (m_autoHide != autoHide)
    {
        m_autoHide = autoHide;
        update();
    }
}

void TwScrollBar::layoutInternalWidgets()
{
    if (m_orientation == Tw::Vertical)
    {
        m_minButton->setBoundRect(0, 0, width(), k_ScrollX);
        m_maxButton->setBoundRect(0, height() - k_ScrollX, width(), k_ScrollX);
    }
    else
    {
        m_minButton->setBoundRect(0, 0, k_ScrollX, height());
        m_maxButton->setBoundRect(width() - k_ScrollX, 0, k_ScrollX, height());
    }
}

TwSize<int> TwScrollBar::hintSize() const
{
    if (m_orientation == Tw::Vertical)
    {
        return TwSize<int>(k_ScrollY, k_ScrollW);
    }
    else
    {
        return TwSize<int>(k_ScrollW, k_ScrollY);
    }
}

void TwScrollBar::paintBackground(TwPainter* painter)
{
    if (m_hovered)
    {
        painter->fillRect(localRect(), k_DefFillColor);
    }
    else
    {
        painter->fillRect(localRect(), k_DefLightFillColor);
    }
}

void TwScrollBar::paint(TwPainter* painter)
{
    if (m_isDragSlider)
    {
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Dark));
    }
    else
    {
        painter->fillRect(m_sliderRect, TwColor::twColor(TwColor::Grey));
    }
}

void TwScrollBar::onMinButtonClicked(TwObject* sender)
{
    TW_UNUSED(sender);
    scrollUp(m_singleStep);
}

void TwScrollBar::onMaxButtonClicked(TwObject* sender)
{
    TW_UNUSED(sender);
    scrollDown(m_singleStep);
}

void TwScrollBar::onMinButtonPressed(TwObject* )
{
    startAutoScrollTimer(Tw::Up, true);
    sigPressed.emit();
}

void TwScrollBar::onMaxButtonPressed(TwObject* )
{
    startAutoScrollTimer(Tw::Down, true);
    sigPressed.emit();
}

void TwScrollBar::onMinButtonReleased(TwObject* )
{
    stopAutoScrollTimer();
}

void TwScrollBar::onMaxButtonReleased(TwObject* )
{
    stopAutoScrollTimer();
}

void TwScrollBar::startAutoScrollTimer(Tw::Direction direction, bool autoModeSingleStep)
{
    m_autoDirection = direction;
    m_autoModeSingleStep = autoModeSingleStep;
    m_autoScrollTimer.start();
}

void TwScrollBar::stopAutoScrollTimer()
{
    m_autoDirection = Tw::NoneDirection;
    m_autoScrollTimer.stop();
}

void TwScrollBar::onAutoScrollTimerUpdate(TwObject*)
{
    if (m_autoDirection == Tw::Up)
    {
        scrollUp( m_autoModeSingleStep ? singleStep() : pageStep());
        if (twCompareValue(m_currentPostion, m_minPostion))
        {
            m_autoScrollTimer.stop();
        }
    }
    else if (m_autoDirection == Tw::Down)
    {
        scrollDown(m_autoModeSingleStep ? singleStep() : pageStep());
        if (twCompareValue(m_currentPostion, m_maxPostion) )
        {
            m_autoScrollTimer.stop();
        }
    }
}

void TwScrollBar::updateSlider()
{
    float currentPos = m_currentPostion;
    if (pageStep() > range())
    {
        currentPos = 0.f;
        scrollToPostion(currentPos);
        if (m_autoHide)
        {
            setVisible(false);
            return;
        }
    }

    setVisible(true);

    updateSliderRect();
}

void TwScrollBar::updateSliderRect()
{
    if (m_maxPostion - m_minPostion < 1.f)
    {
        m_sliderRect = TwRect<int>();
    }
    else
    {
        if (m_orientation == Tw::Vertical)
        {
            int sliderHeight = static_cast<int>(pageStep() / (m_maxPostion - m_minPostion) * (height() - k_ScrollX * 2));
            int sliderPos = static_cast<int>(currentPostion() / (m_maxPostion - m_minPostion) *(height() - k_ScrollX * 2 - sliderHeight)) + k_ScrollX;

            m_sliderRect.setLeft(0);
            m_sliderRect.setTop(sliderPos);
            m_sliderRect.setWidth(width());
            m_sliderRect.setHeight(sliderHeight);
        }
        else
        {
            int sliderWidth = static_cast<int>(pageStep() / (m_maxPostion - m_minPostion) * (width() - k_ScrollX * 2));
            int sliderPos = static_cast<int>(currentPostion() / (m_maxPostion - m_minPostion) *(width() - k_ScrollX * 2 - sliderWidth)) + k_ScrollX;

            m_sliderRect.setLeft(sliderPos);
            m_sliderRect.setTop(0);
            m_sliderRect.setWidth(sliderWidth);
            m_sliderRect.setHeight(height());
        }
    }
    update();
}

void TwScrollBar::onPostionChanged(float oldPostion)
{
    TW_UNUSED(oldPostion);
    updateSliderRect();
}

void TwScrollBar::onResizeEvent(TwResizeEvent* ev)
{
    TwWidget::onResizeEvent(ev);
    updateSliderRect();
}

void TwScrollBar::onMousePresseEvent(TwMouseEvent* ev)
{
    ev->accept();
    if (m_sliderRect.contains(ev->localPos()))
    {
        m_isDragSlider = true;
        m_dragStartPoint = ev->localPos();
        m_dragStartPostion = m_currentPostion;
    }
    else
    {
        if (m_orientation == Tw::Vertical)
        {
            TwRect<int> upRect(0, k_ScrollX, width(), m_sliderRect.top());
            TwRect<int> downRect(0, m_sliderRect.bottom(), width(), height() - k_ScrollX);
            if (upRect.contains(ev->localPos()))
            {
                scrollUp(pageStep());
                startAutoScrollTimer(Tw::Up, false);
            }
            else if (downRect.contains(ev->localPos()))
            {
                scrollDown(pageStep());
                startAutoScrollTimer(Tw::Down, false);
            }
        }
        else
        {
            TwRect<int> leftRect(k_ScrollX, 0, m_sliderRect.left(), height());
            TwRect<int> rightRect(m_sliderRect.right(), 0, width()- k_ScrollX, height());
            if (leftRect.contains(ev->localPos()))
            {
                scrollUp(pageStep());
                startAutoScrollTimer(Tw::Up, false);
            }
            else if (rightRect.contains(ev->localPos()))
            {
                scrollDown(pageStep());
                startAutoScrollTimer(Tw::Down, false);
            }
        }
    }

    sigPressed.emit();

    update();
}

void TwScrollBar::onMouseReleaseEvent(TwMouseEvent* ev)
{
    TW_UNUSED(ev);
    m_isDragSlider = false;
    stopAutoScrollTimer();
    update();
}

void TwScrollBar::onMouseMoveEvent(TwMouseEvent* ev)
{
    if (m_isDragSlider)
    {
        TwPoint<int> delta = ev->localPos() - m_dragStartPoint;
        if (m_orientation == Tw::Vertical)
        {
            float step = (m_maxPostion - m_minPostion)* delta.y() / (height() - k_ScrollX * 2 - m_sliderRect.height());
            scrollToPostion(m_dragStartPostion + step);
        }
        else
        {
            float step = (m_maxPostion - m_minPostion) * delta.x() / (width() - k_ScrollX * 2 - m_sliderRect.width());
            scrollToPostion(m_dragStartPostion + step);
        }
        update();
    }
}

void TwScrollBar::onHoverEnterEvent(TwHoverEvent* ev)
{
    TW_UNUSED(ev);
    m_hovered = true;
    update();
}

void TwScrollBar::onHoverLeaveEvent(TwHoverEvent* ev)
{
    TW_UNUSED(ev);
    m_hovered = false;
    update();
}

void TwScrollBar::onWheelEvent(TwWheelEvent* ev)
{
    int offset = ev->offset();

    if (m_currentPostion <= m_minPostion && offset > 0)
    {
        return;
    }
    if (m_currentPostion >= m_maxPostion && offset < 0)
    {
        return;
    }

    int stepCount = -offset / TwWheelEvent::wheeldelta;
    scroll( static_cast<float>( stepCount ) * m_singleStep );

    ev->accept();
}

