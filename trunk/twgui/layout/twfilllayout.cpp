#include "stable.h"
#include "twwidget.h"

#include "twfilllayout.h"


TwFillLayout::TwFillLayout(TwObject* parent)
    : TwLayoutManager(parent)
{

}

TwFillLayout::~TwFillLayout()
{

}

void TwFillLayout::onWidgetAdd(TwWidget* child)
{
    TW_UNUSED(child);
    DAssert(m_hostWidget->widgetCount() <= 1);
}

void TwFillLayout::onWidgetRemove(TwWidget* child)
{
    TW_UNUSED(child);
    DAssert(m_hostWidget->widgetCount() <= 1);
}

void TwFillLayout::layoutWidgets()
{
    DAssert(m_hostWidget->widgetCount() <= 1);
    if (m_hostWidget->widgetCount())
    {
        TwWidget* w = m_hostWidget->widgetAt(0);
        w->setBoundRect(m_hostWidget->localRect().adjust(m_hostWidget->contentMargin()));
    }
}


void TwFillLayout::setupGeometry()
{
    if (m_dirty)
    {
        TwWidget* w = m_hostWidget->widgetAt(0);
        if (w)
        {
            m_cachePerredSize = w->calcLayoutSize();
            m_cachePerredSize.setWidth(m_cachePerredSize.width() + m_hostWidget->contentMargin().left() + m_hostWidget->contentMargin().right());
            m_cachePerredSize.setHeight(m_cachePerredSize.height() + m_hostWidget->contentMargin().top() + m_hostWidget->contentMargin().bottom());
        }
        m_dirty = false;
    }
}


