#include "stable.h"
#include "twscrollview.h"


TW_CLASS_IMPLEMENT(TwScrollView, TwWidget);

const int k_ScrollBarWidth = 10;

TwScrollView::TwScrollView(TwWidget* parent /*= nullptr*/)
    : TwWidget(parent)
    , m_scrollBar(nullptr)
    , m_contentWidget(nullptr)
{
    m_scrollBar = new TwScrollBar(this);
    m_scrollBar->setZOrder(1);
    m_scrollBar->setVisible(false);
    m_scrollBar->sigVisibleChanged.connect(this, &TwScrollView::onScrollVisbleChanged);
    m_scrollBar->sigScrollPostionChanged.connect(this, &TwScrollView::onScrollBarPostionChanged);
}

TwScrollView::~TwScrollView()
{

}

void TwScrollView::setContentWidget(TwWidget* w)
{
    if (m_contentWidget != w)
    {
        if (m_contentWidget)
        {
            m_contentWidget->setParentWidget(nullptr);
            m_contentWidget->removeEventHook(this);
        }

        m_contentBoundChangedSigConnection->expire();
        m_contentBoundChangedSigConnection.reset();

        m_contentWidget = w;
        if (m_contentWidget)
        {
            m_contentWidget->setParentWidget(this);
            m_contentWidget->setPos(0, 0);
            m_contentWidget->setZOrder(-1);
            m_contentBoundChangedSigConnection = m_contentWidget->sigBoundChanged.connect(this, &TwScrollView::onContentWidgetBoundChanged);
            m_contentWidget->installEventHook(this);
        }

        onContentWidgetChanged();
    }
}

void TwScrollView::setSingleStep(float singleStep)
{
    m_scrollBar->setSingleStep(singleStep);
}

void TwScrollView::scrollToPostion(float postion)
{
    m_scrollBar->scrollToPostion(postion);
}

void TwScrollView::scrollToMin()
{
    m_scrollBar->scrollToMin();
}

void TwScrollView::scrollToMax()
{
    m_scrollBar->scrollToMax();
}

TwSize<int> TwScrollView::hintSize() const
{
    return TwSize<int>(50, 50);
}

void TwScrollView::onScrollVisbleChanged(TwObject* sender)
{
    TW_UNUSED(sender);
    
    updateScrollView();
}

void TwScrollView::onScrollBarPostionChanged(TwObject* sender, float position)
{
    TW_UNUSED(sender);
    m_contentWidget->setY(static_cast<int>(-position));

    updateScrollView();
}

void TwScrollView::onScrollBarPressed(TwObject* sender)
{
    TW_UNUSED(sender);
    if (m_contentWidget)
    {
        m_contentWidget->setFocus();
    }
}

void TwScrollView::onContentWidgetChanged()
{
    float max = static_cast<float>(m_contentWidget->height() - height());
    if (max < 0)
    {
        max = 0;
    }
    m_scrollBar->setRange(0.f, max);
    m_scrollBar->setPageStep(static_cast<float>(height()));
    m_scrollBar->scrollToPostion(0.f);

    updateScrollView();
}

void TwScrollView::onContentWidgetBoundChanged(TwObject* sender)
{
    TW_UNUSED(sender);
    float max = static_cast<float>(m_contentWidget->height() - height());
    if (max < 0)
    {
        max = 0;
    }
    m_scrollBar->setRange(0.f, max);
    m_scrollBar->setPageStep(static_cast<float>(height()));

    updateScrollView();
}

void TwScrollView::updateScrollView()
{
    if (m_contentWidget)
    {
        if (m_scrollBar->isVisible())
        {
            m_contentWidget->setWidth(width() - m_scrollBar->width());
        }
        else
        {
            m_contentWidget->setWidth(width());
        }

        TwRect<int> contentVisibleRc;
        contentVisibleRc.setLeft(m_contentWidget->x());
        contentVisibleRc.setTop(static_cast<int>(m_scrollBar->currentPostion()));
        contentVisibleRc.setHeight(height());
        contentVisibleRc.setWidth(m_contentWidget->width());

        updateContentVisibleBound(contentVisibleRc);
    }
}

void TwScrollView::updateContentVisibleBound(const TwRect<int>& rc)
{
    TW_UNUSED(rc);
}

bool TwScrollView::hookHandleEvent(TwEventPort* p, TwEvent* ev)
{
    if (p == m_contentWidget)
    {
        if (ev->id() == Tw::Ev_Wheel)
        {
            this->onWheelEvent(static_cast<TwWheelEvent*>(ev));
            return true;
        }
    }

    return false;
}

void TwScrollView::onResizeEvent(TwResizeEvent* ev)
{
    m_scrollBar->setBoundRect(width() - k_ScrollBarWidth, 0, k_ScrollBarWidth, height());
    m_scrollBar->setPageStep(static_cast<float>(ev->newSize().height()));

    updateScrollView();
}

void TwScrollView::onWheelEvent(TwWheelEvent* ev)
{
    m_scrollBar->onWheelEvent(ev);
}

