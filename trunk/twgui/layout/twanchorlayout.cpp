#include "stable.h"
#include "twwidget.h"

#include "twanchorlayout.h"


TwAnchorLayout::TwAnchorLayout(TwObject* parent)
    : TwLayoutManager(parent)
{

}

TwAnchorLayout::~TwAnchorLayout()
{

}

void TwAnchorLayout::onWidgetAdd(TwWidget* child)
{
    TwLayoutManager::onWidgetAdd(child);
}

void TwAnchorLayout::onWidgetRemove(TwWidget* child)
{
    TwLayoutManager::onWidgetRemove(child);
}

void TwAnchorLayout::layoutWidgets()
{
    
}

void TwAnchorLayout::setupGeometry()
{
    //TODO
    if (m_dirty)
    {
        m_cachePerredSize = m_hostWidget->size();
        m_dirty = false;
    }
}
