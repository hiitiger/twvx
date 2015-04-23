#include "stable.h"
#include "twprogressbar.h"


TW_CLASS_IMPLEMENT(TwProgressBar, TwWidget);

TwProgressBar::TwProgressBar(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , sigProgressChanged(this)
    , sigProgressCompleted(this)
    , m_color(TwColor::twColor(TwColor::LightSkyBlue))
    , m_progressValue(0.f)
{

}

TwProgressBar::~TwProgressBar()
{

}

void TwProgressBar::setProgressValue(float progress)
{
    if (progress < 0.f)
    {
        progress = 0.f;
    }
    if (progress > 1.f)
    {
        progress = 1.0f;
    }

    if (!twCompareValue(m_progressValue, progress))
    {
        float oldProgress = m_progressValue;
        m_progressValue = progress;
        onProgressChanged(oldProgress);

        sigProgressChanged.emit(m_progressValue);

        if (twCompareValue(m_progressValue, 1.0f))
        {
            sigProgressCompleted.emit();
        }
    }
}

void TwProgressBar::setProgressColor(TwColor color)
{
    m_color = color;
}

TwSize<int> TwProgressBar::hintSize() const
{
    return TwSize<int>(100, 20);
}

void TwProgressBar::paintBackground(TwPainter* painter)
{
    TwWidget::paintBackground(painter);

    painter->drawRect(localRect(), k_DefBoundColor, 1.0f, true);
}

void TwProgressBar::paint(TwPainter* painter)
{
    TwRect<int> rc = localRect();
    rc.setWidth((int)(m_progressValue / 1.0f * width()));
    painter->fillRect(rc.adjust(1), m_color);
}
