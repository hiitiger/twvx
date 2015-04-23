#include "stable.h"
#include "twwidget.h"

#include "twboxlayout.h"


TwBoxLayout::TwBoxLayout(TwObject* parent, Tw::Orientation o)
    : TwLayoutManager(parent)
    , m_orientation(o)
{

}

TwBoxLayout::~TwBoxLayout()
{

}

void TwBoxLayout::onWidgetAdd(TwWidget* child)
{
    TwLayoutManager::onWidgetAdd(child);
}

void TwBoxLayout::onWidgetRemove(TwWidget* child)
{
    TwLayoutManager::onWidgetAdd(child);
}

void TwBoxLayout::layoutWidgets()
{
    TwRect<int> rect = m_hostWidget->localRect();
    TwMargin<int> contentMargin = m_hostWidget->contentMargin();
    int childWidgetSpacing  = m_hostWidget->childSpacing();

    int x = rect.left() + contentMargin.left();
    int y = rect.top() + contentMargin.top();
    rect.moveTo(x, y);
    rect.resize(rect.width() - contentMargin.left() - contentMargin.right(), rect.height() - contentMargin.top() - contentMargin.bottom());

    //TODO

    for(int i = 0; i < m_hostWidget->widgetCount(); ++i)
    {
        TwWidget* child = m_hostWidget->widgetAt(i);

        if(child->isVisible())
        {
            TwRect<int> boundRect(x, y, rect.right(), rect.bottom());

            TwSize<int> childSize = child->calcLayoutSize();

            if (m_orientation == Tw::Horizontal)
            {
                boundRect.setWidth(childSize.width());
               // boundRect.setHeight(twMin(boundRect.height(), childSize.height()));

                x += childSize.width() + childWidgetSpacing;
            }
            else
            {
                boundRect.setHeight(childSize.height());
               // boundRect.setWidth(twMin(boundRect.width(), childSize.width()));

                y += childSize.height() + childWidgetSpacing;
            }

            child->setBoundRect(boundRect);
        }
    }
}

void TwBoxLayout::setupGeometry()
{
    if (m_dirty)
    {
        //TODO
        TwRect<int> rect = m_hostWidget->localRect();
        TwMargin<int> contentMargin = m_hostWidget->contentMargin();
        TwRect<int> boundRect;
        int childWidgetSpacing = m_hostWidget->childSpacing();

        boundRect.resize(contentMargin.left() + contentMargin.right(), contentMargin.top() + contentMargin.bottom());
        TwSize<int> maxChildWidthHeight;

        int visibleChildCount = 0;
        for (int i = 0; i < m_hostWidget->widgetCount(); ++i)
        {
            TwWidget* child = m_hostWidget->widgetAt(i);

            if (child->isVisible())
            {
                visibleChildCount += 1;
                TwSize<int> childSize = child->calcLayoutSize();

                if (m_orientation == Tw::Horizontal)
                {
                    boundRect.enlargeWidth(childSize.width());
                    maxChildWidthHeight.setHeight(twMax(maxChildWidthHeight.height(), childSize.height()));
                }
                else
                {
                    boundRect.enlargeHeight(childSize.height());
                    maxChildWidthHeight.setWidth(twMax(maxChildWidthHeight.width(), childSize.width()));
                }
            }
        }

        boundRect.enlarge(maxChildWidthHeight.width(), maxChildWidthHeight.height());
        if (visibleChildCount > 0)
        {
            if (m_orientation == Tw::Horizontal)
            {
                boundRect.enlargeWidth(childWidgetSpacing * (visibleChildCount - 1));
            }
            else
            {
                boundRect.enlargeHeight(childWidgetSpacing * (visibleChildCount - 1));
            }
        }

        m_cachePerredSize = boundRect.size();
        m_dirty = false;
    }
}
